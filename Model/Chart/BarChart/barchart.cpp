#include "barchart.h"
#include <cstdlib>
#include <QJsonArray>

// ------------------- SET ----------------------------------------------------

Set::Set(int values_size) : name(""), color(rand()%256, rand()%256, rand()%256), values(values_size, 0){}

Set::Set(const QString &n, vector<double> &arr) : name(n),  color(rand()%256, rand()%256, rand()%256), values(arr){}

Set::Set(const QJsonObject &obj) : name(obj.value("name").toString()), color(obj.value("color").toString()){
    QJsonArray jvalues = obj.value("values").toArray();
    for(QJsonArray::const_iterator i = jvalues.begin(); i != jvalues.end(); ++i)
        values.push_back((i->toDouble() >= 0) ? i->toDouble() : 0);
}

QString Set::getName() const{
    return name;
}

bool Set::changeValueOfCategoryAt(int cat_index, double new_value){
    bool result = false;
    if(cat_index >= 0 && cat_index < values.size()){
        values[cat_index] = new_value;
        result = true;
    }
    return result;
}

void Set::changeColor(const QColor &new_color){
    color = new_color;
}

QColor Set::getColor() const{
    return color;
}

double Set::getValueOfCategoryAt(int cat_index){
    if(cat_index >= 0 && cat_index < values.size())
        return values[cat_index];
    return 0;
}

void Set::changeName(const QString &new_name){
    name = new_name;
}

void Set::insertValue(int index, double value){
    if(index >= 0 && index <= values.size())
        values.insert(values.begin()+index, value);
}

bool Set::removeValuesAt(int index, int count){
    bool result = false;
    if(index >= 0 && count > 0 && index+count-1 < values.size()){
        values.erase(values.begin()+index, values.begin()+index+count);
        result = true;
    }
    return result;
}

int Set::getValuesSize() const{
    return values.size();
}

QJsonObject Set::parsing() const{
    QJsonObject json_set;
    QJsonArray json_values;
    json_set.insert("name", name);
    json_set.insert("color", color.name());
    for(vector<double>::const_iterator i = values.begin(); i != values.end(); ++i)
        json_values.push_back(*i);
    json_set.insert("values", json_values);
    return json_set;
}

// --------------------- BAR CHART ----------------------------------------------

vector<Set*> BarChart::copySets(const BarChart &from){
    vector<Set*> result;
    for(vector<Set*>::const_iterator i = from.sets.begin(); i != from.sets.end(); ++i)
        result.push_back(new Set(**i));
    return result;
}

void BarChart::destroySets(BarChart &from){
    for(vector<Set*>::iterator i = from.sets.begin(); i != from.sets.end(); ++i)
        delete *i;
}

bool BarChart::isSetNameValid(const QString &name) const{
    bool result = true;
    for(vector<Set*>::const_iterator i = sets.begin(); i != sets.end() && result; ++i)
        result = (*i)->getName().compare(name, Qt::CaseInsensitive);
    return result;
}

bool BarChart::isCategoryNameValid(const QString &name) const{
    bool result = !name.isEmpty();
    for(vector<QString>::const_iterator i = categories.begin(); i != categories.end() && result; ++i)
        result = i->compare(name, Qt::CaseInsensitive);
    return result;
}

bool BarChart::insertNewCategoryValues(int index){
    bool result = false;
    if(index >= 0 && index <= categories.size()){
        for(int i = 0; i < sets.size(); ++i)
            sets[i]->insertValue(index, 0);
        result = true;
    }
    return result;
}

bool BarChart::removeAllCategoryValues(int cat_index){
    bool result = true;
    for(vector<Set*>::iterator i = sets.begin(); i != sets.end() && result; ++i)
        result = (*i)->removeValuesAt(cat_index, 1);
    return result;
}

void BarChart::getSetsFromJson(const QJsonArray &jsets){
    int invalid_name = 0;
    for(QJsonArray::const_iterator i = jsets.begin(); i != jsets.end(); ++i){
        QJsonObject current_set = i->toObject();
        if(!isSetNameValid(current_set.value("name").toString())){
            current_set.insert("name", "invalid name "+QString::number(invalid_name));
            invalid_name++;
        }
        sets.push_back(new Set(i->toObject()));
    }
}

void BarChart::getCategoriesNameJson(const QJsonArray& jcategories){
    int invalid_name = 0;
    for(QJsonArray::const_iterator i = jcategories.begin(); i != jcategories.end(); ++i){
        QString cat_name = i->toString();
        if(!isCategoryNameValid(cat_name)){
            cat_name = "invalid name "+QString::number(invalid_name);
            invalid_name++;
        }
        categories.push_back(cat_name);
    }
}

bool BarChart::isBarChartConsistent(const QJsonObject &obj){
    bool result = true;
    QJsonArray jcat = obj.value("cat").toArray();
    QJsonArray jsets = obj.value("sets").toArray();
    if(!jsets.isEmpty()){
        int set_values_size = jsets.at(0).toObject().value("values").toArray().size();
        for(QJsonArray::const_iterator i = jsets.begin(); i != jsets.end() && result; ++i)
            result = set_values_size == i->toObject().value("values").toArray().size();
        if(result)
            result = jcat.size() == set_values_size;
    }
    return result;
}

BarChart::BarChart(const QString &title) : Chart(title){}

BarChart::BarChart(const QJsonObject &obj) : Chart(obj){
    if(isBarChartConsistent(obj)){
        getSetsFromJson(obj.value("sets").toArray());
        getCategoriesNameJson(obj.value("cat").toArray());
    }
}

BarChart::BarChart(const BarChart &chart) : Chart(chart), sets(copySets(chart)){}

BarChart& BarChart::operator =(const BarChart& chart){
    if(this != & chart){
        destroySets(*this);
        sets = copySets(chart);
    }
    return *this;
}

BarChart::~BarChart(){
    destroySets(*this);
}

bool BarChart::changeSetNameAt(int set_index, const QString &new_name){
    bool result = false;
    if(set_index >= 0 && set_index < sets.size() && isSetNameValid(new_name)){
        sets[set_index]->changeName(new_name);
        result = true;
    }
    return result;
}

bool BarChart::changeCategoryNameAt(int cat_index, const QString &new_name){
    bool result = false;
    if(cat_index >= 0 && cat_index < categories.size() && isCategoryNameValid(new_name)){
        categories[cat_index] = new_name;
        result = true;
    }
    return result;
}

bool BarChart::insertCategories(int index, int count){
    bool result = false;
    if(index >= 0 && index <= categories.size()){
        for(int i = 0; i < count; ++i){
            categories.insert(categories.begin()+index+i, QString());
            insertNewCategoryValues(index);
        }
        result = true;
    }
    return result;
}

bool BarChart::insertSetsAt(int index, int count){
    bool result = false;
    if(index >= 0 && index <= sets.size() && count > 0){
        for(int i = 0; i < count; ++i)
            sets.insert(sets.begin()+index+i, new Set(categories.size()));
        result = true;
    }
    return result;
}

bool BarChart::removeCategoryAt(int cat_index){
    bool result = false;
    if(removeAllCategoryValues(cat_index)){
        categories.erase(categories.begin()+cat_index);
        result = true;
    }
    return result;
}

bool BarChart::removeSetsAt(int set_index, int count){
    bool result = false;
    if(set_index >= 0 && count > 0 && set_index+count-1 < sets.size()){
        for(int i = 0; i < count; ++i){
            delete sets[set_index+i];
            sets.erase(sets.begin()+set_index+i);
            result = true;
        }
    }
    return result;
}

Set* BarChart::getSetAt(int index){
    return (index >= 0 && index < sets.size()) ? sets[index] : nullptr;
}

QString BarChart::getCategoryNameAt(int index) const{
    return (index >= 0 && index < categories.size()) ? categories[index] : QString();
}

int BarChart::setsSize() const{
    return sets.size();
}

int BarChart::categoriesSize() const{
    return categories.size();
}

QJsonObject* BarChart::parsing() const{
    QJsonObject *obj = Chart::parsing();
    obj->insert("type", "bar");
    QJsonArray json_sets, json_cat;
    for(vector<Set*>::const_iterator i = sets.cbegin(); i != sets.cend(); ++i){
        json_sets.push_back((*i)->parsing());
    }
    for(vector<QString>::const_iterator i = categories.cbegin(); i != categories.cend(); ++i)
        json_cat.push_back(*i);
    obj->insert("sets", json_sets);
    obj->insert("cat", json_cat);
    return obj;
}


