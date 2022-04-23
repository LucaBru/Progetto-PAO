#include "piechart.h"
#include <QJsonArray>

// ------------- Slice ------------------------
Slice::Slice(const QString &n, double v, const QColor c) : name(n), value(v), color(c){}

Slice::Slice(const QJsonObject &obj) : name(obj.value("name").toString()), value(obj.value("value").toDouble()), color(obj.value("color").toString()){}

QString Slice::getName() const{
    return name;
}

double Slice::getValue() const{
    return value;
}

QColor Slice::getColor() const{
    return color;
}

void Slice::changeName(const QString &newName){
    name = newName;
}

void Slice::changeValue(double newValue){
    value = newValue;
}

void Slice::changeColor(const QColor &new_color){
    color = new_color;
}

// -------------- PieChart -------------------

PieChart::PieChart(const QString &t) : Chart(t){}

PieChart::PieChart(const QJsonObject& obj) : Chart(obj) {
    QJsonArray list_of_slices = obj.value("slices").toArray();
    for(QJsonArray::const_iterator i = list_of_slices.begin(); i != list_of_slices.end(); ++i)
        slices.push_back(new Slice(i->toObject()));
}

vector<Slice*> PieChart::copySlices(const PieChart & from){
    vector<Slice*> result;
    for(vector<Slice*>::const_iterator i = from.slices.begin(); i != from.slices.end(); ++i){
            result.push_back(new Slice(**i));
    }
    return result;
}

void PieChart::deleteSlices(PieChart & pieChart){
    for(vector<Slice*>::iterator i = pieChart.slices.begin(); i != pieChart.slices.end(); ++i)
        delete *i;
}

bool PieChart::isSliceNameValid(const QString& name) const{
    bool valid = true;
    for(vector<Slice*>::const_iterator i = slices.begin(); valid && i != slices.end(); ++i)
        valid = (*i)->getName().compare(name);
    return valid;
}

PieChart::PieChart(const PieChart &from) : Chart(from), slices(PieChart::copySlices(from)){}

PieChart& PieChart::operator =(const PieChart& from){
    if(this != &from){
        deleteSlices(*this);
        slices = copySlices(from);
        title = from.title;
    }
    return *this;
}

PieChart::~PieChart(){
    deleteSlices(*this);
}

bool PieChart::insertSlices(int row, int count){
    if(row >= 0 && row <= slices.size() && count > 0){
        for(int i = 0; i < count; --count)
            slices.insert(slices.begin()+row+i, new Slice());
        return true;
    }
    return false;
}

bool PieChart::removeSlices(int row, int count){
    if(row >= 0 && count >= 0 && row+count-1 < slices.size()){
        for(vector<Slice*>::iterator i = slices.begin()+row; count > 0; --count){
            delete *i;
            i = slices.erase(slices.begin()+row, slices.begin()+row+count);
        }
        return true;
    }
    return false;
}

Slice* PieChart::getSlice(int index) const{
    return (index >= 0 && index < slices.size()) ? slices[index] : nullptr;
}

int PieChart::slicesCount() const{
    return slices.size();
}

bool PieChart::changeSliceAtName(int index, const QString &new_name){
    bool result = false;
    if(index >= 0 && index < slices.size() && isSliceNameValid(new_name)){
        slices[index]->changeName(new_name);
        result = true;
    }
    return result;
}

QJsonObject* PieChart::parsing() const {
    QJsonObject *obj = Chart::parsing();
    obj->insert("type", "pie");
    QJsonArray slices_in_chart;
    for(vector<Slice*>::const_iterator i = slices.begin(); i != slices.end(); ++i){
        QJsonObject slice;
        slice.insert("name", (*i)->getName());
        slice.insert("value", (*i)->getValue());
        slice.insert("color", (*i)->getColor().name());
        slices_in_chart.append(slice);
    }
    obj->insert("slices", slices_in_chart);
    return obj;
}



















