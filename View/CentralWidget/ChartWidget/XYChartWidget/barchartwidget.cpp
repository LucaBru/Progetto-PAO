#include "barchartwidget.h"
#include <QIcon>
#include <QHBoxLayout>
#include "Model/XYModel/barmodel.h"
#include <QInputDialog>
#include <QColorDialog>
#include <QColor>
#include <QFrame>
#include <iostream>
using std::cout;

// ---------------------- CATEGORYWIDGET -----------------------------------------------

void CategoryWidget::connectSignalsAndSlots() const{
    QObject::connect(garbage, SIGNAL(clicked(bool)), this, SLOT(userRemoveCategory()));
    QObject::connect(category_name, SIGNAL(returnPressed()), this, SLOT(userChangeCategoryName()));
    QObject::connect(set_value, SIGNAL(returnPressed()), this, SLOT(userChangeSetValue()));
    QObject::connect(add_new_category, SIGNAL(clicked(bool)), this, SLOT(userAddCategory()));
}

void CategoryWidget::confWidgetItems(){
    category_name->setPlaceholderText("Category name");
    QDoubleValidator *cat_val = new QDoubleValidator(this);
    cat_val->setBottom(0);
    set_value->setValidator(cat_val);
    set_value->setPlaceholderText("0");
    garbage->setIcon(QIcon("..\\Chart-Application\\icon\\garbage.png"));
    add_new_category->setIcon(QIcon("..\\Chart-Application\\icon\\plus.png"));
    QHBoxLayout *l = new QHBoxLayout(this);
    l->addWidget(garbage);
    l->addWidget(category_name);
    l->addWidget(set_value);
    l->addWidget(add_new_category);
}

CategoryWidget::CategoryWidget(BarChartWidget *p, int i) : QWidget(p), parent(p),  garbage(new QToolButton()), category_name(new QLineEdit()), set_value(new QLineEdit()), add_new_category(new QToolButton()), index(i){
    connectSignalsAndSlots();
    confWidgetItems();
}

void CategoryWidget::setValue(double val){
    set_value->setText(QString::number((val)));
}

void CategoryWidget::setCategoryName(const QString &name){
    category_name->setText(name);
}

void CategoryWidget::setBorderForTextError(){
    category_name->setStyleSheet("border: 1px solid red");
}

void CategoryWidget::setDefaultBorder(){
    category_name->setStyleSheet("");
}

void CategoryWidget::userRemoveCategory(){
    parent->removeCategory(index);
}

void CategoryWidget::userAddCategory(){
    parent->addCategory(index+1);
}

void CategoryWidget::userChangeCategoryName(){
    parent->changeCategoryName(index, category_name->text());
}

void CategoryWidget::userChangeSetValue(){
    parent->changeSetValue(index, set_value->text().toDouble());
}


// --------------------- BARCHARTWIDGET -------------------------------------------------

void BarChartWidget::connectSignalsAndSlots() const{
    QObject::connect(add_serie, SIGNAL(clicked(bool)), this, SLOT(userInsertSet()));
    QObject::connect(remove_serie, SIGNAL(clicked(bool)), this, SLOT(userRemoveSet()));
    QObject::connect(add_new_category, SIGNAL(clicked(bool)), this, SLOT(userInsertFirstCategory()));
    QObject::connect(series, SIGNAL(currentIndexChanged(int)), this, SLOT(currentSetChanged(int)));
    QObject::connect(set_name, SIGNAL(returnPressed()), this, SLOT(userChangeSetName()));
    QObject::connect(bar_serie, SIGNAL(clicked(int, QBarSet*)), this, SLOT(barClicked(int, QBarSet*)));
    QObject::connect(color, SIGNAL(clicked(bool)), this, SLOT(changeColor()));
}

void BarChartWidget::connectBarModelSignals() const{
    BarModel *m = dynamic_cast<BarModel*>(model);
    if(m){
        QObject::connect(m, SIGNAL(multipleSetsInserted(int,int)), this, SLOT(multipleSetsInserted(int,int)));
        QObject::connect(m, SIGNAL(multipleCategoriesInserted(int,int)), this, SLOT(multipleCategoriesInserted(int,int)));
        QObject::connect(m, SIGNAL(multipleSetsRemoved(int,int)), this, SLOT(multipleSetsRemoved(int,int)));
        QObject::connect(m, SIGNAL(multipleCategoriesRemoved(int,int)), this, SLOT(multipleCategoriesRemoved(int,int)));
        QObject::connect(m, SIGNAL(setAtChangedName(int,QString)), this, SLOT(setAtChangedName(int,QString)));
        QObject::connect(m, SIGNAL(setAtChangedValue(int,int,double)), this, SLOT(setAtChangedValue(int,int,double)));
        QObject::connect(m, SIGNAL(categoryAtChangedName(int,QString)), this, SLOT(categoryAtChangedName(int,QString)));
    }
}

void BarChartWidget::configChartWidgetItems(){
    chart_info->setTitle("Bar Chart Info");
    serie_info->setTitle("Set Info");
    add_serie->setText("Add Set");
    remove_serie->setText("Remove Set");
    chart_info_layout->insertRow(1, "Sets", series);
    main_layout->setRowStretch(1, 1);
}

void BarChartWidget::configBarChartWidgetItems(){
    QHBoxLayout *set_name_layout = new QHBoxLayout();
    set_name_layout->addWidget(set_name);
    set_name_layout->addWidget(color);
    set_name->setPlaceholderText("set name");
    serie_info_layout->addRow(set_name_layout);
    if(cat_items_layout->rowCount() == 0)
        serie_info_layout->addRow(add_new_category);
    QSizePolicy p;
    p.setHorizontalPolicy(QSizePolicy::Expanding);
    p.setVerticalPolicy(QSizePolicy::Expanding);
    p.setVerticalStretch(1);
    QScrollArea *area = new QScrollArea();
    QWidget *tmp = new QWidget();
    serie_info_layout->addRow(area);
    area->setWidgetResizable(true);
    area->setWidget(tmp);
    tmp->setLayout(cat_items_layout);
    area->setSizePolicy(p);
    area->setFrameShape(QFrame::NoFrame);
}

void BarChartWidget::updateCategoryWidgetItems(int index, int count){
    for(int i = index; i < index+count; ++i)
        cat_items_layout->insertRow(i, new CategoryWidget(this, i));
    updateIndexOfCategoryWidgetItems(index, count);
}

void BarChartWidget::updateIndexOfCategoryWidgetItems(int index, int count){
    for(int i = index+count; i < cat_items_layout->rowCount(); ++i){
        CategoryWidget *c = dynamic_cast<CategoryWidget*>(cat_items_layout->itemAt(i, QFormLayout::SpanningRole)->widget());
        if(c)
            c->index = i;
    }
}

void BarChartWidget::updateSetValueOnSeriesCurrentIndexChanged(int index){
    if(index >= 0 && index < bar_serie->count()){
        QBarSet* set = bar_serie->barSets()[index];
        for(int i = 0; i < set->count(); ++i){
            CategoryWidget *current_cat_widget = dynamic_cast<CategoryWidget*>(cat_items_layout->itemAt(i, QFormLayout::SpanningRole)->widget());
            if(current_cat_widget)
                current_cat_widget->setValue(set->at(i));
        }
    }
}

void BarChartWidget::inizializeNewCategorySetValue(int column, int count){
    QList<QBarSet*> sets = bar_serie->barSets();
    for(QList<QBarSet*>::iterator i = sets.begin(); i != sets.end(); ++i){
        if(column+count-1 <= (*i)->count())
            for(int j = 0; j < count; ++j)
                (*i)->insert(column+j, 0);
    }

}

void BarChartWidget::removeSetsValueAtIndex(int index){
    QList<QBarSet*> sets = bar_serie->barSets();
    bool is_max_removed = false;
    QValueAxis *y = static_cast<QValueAxis*>(y_axis);
    double max = y->max();
    for(QList<QBarSet*>::iterator i = sets.begin(); i != sets.end(); ++i){
        is_max_removed = (!is_max_removed) ? (*i)->at(index)*5/4 == max : true;
        (*i)->remove(index);
    }
    if(is_max_removed)
        y->setMax(findNewValueAxisMaxWithOutSet());
}

void BarChartWidget::getSetsFromModel(){
    QValueAxis *y = static_cast<QValueAxis*>(y_axis);
    for(int i = 0; i < model->rowCount(); ++i){
        QModelIndex index = model->index(i, 0);
        QBarSet *set = new QBarSet(model->data(index).toString());
        set->setColor(model->data(index, Qt::DecorationRole).value<QColor>());
        for(int j = 0; j < model->columnCount()-1; ++j){
            double value = model->data(model->index(i, j+1)).toDouble();
            set->append(value);
            if(value*5/4 > y->max())
                y->setMax(value*5/4);
        }
        bar_serie->append(set);
    }
}

void BarChartWidget::getCategoriesFromModel(){
    if(model->columnCount()-1 > 0){
        static_cast<QBarCategoryAxis*>(x_axis)->remove("");
        serie_info_layout->removeRow(add_new_category);
        QBarCategoryAxis *categories_axis = static_cast<QBarCategoryAxis*>(x_axis);
        for(int i = 0; i < model->columnCount()-1; ++i){
            QString cat_name = static_cast<BarModel*>(model)->getCategoryNameAt(i);
            categories_axis->append(cat_name);
            CategoryWidget *cat_item = new CategoryWidget(this, i);
            cat_item->setCategoryName(cat_name);
            cat_items_layout->addRow(cat_item);
        }
    }
}

void BarChartWidget::changeSetColor(int set_index){
    QBarSet *set = bar_serie->barSets()[set_index];
    QColor new_color = QColorDialog::getColor(set->color(), this, "Choose new set color");
    if(new_color.isValid())
        if(model->setData(model->index(set_index, 0), new_color, Qt::DecorationRole))
            set->setColor(new_color);
}

void BarChartWidget::resetCategoryBorderStyle(){
    QBarCategoryAxis *categories_axis = static_cast<QBarCategoryAxis*>(x_axis);
    for(int i = 0; i < categories_axis->count(); ++i)
        static_cast<CategoryWidget*>(cat_items_layout->itemAt(i, QFormLayout::SpanningRole)->widget())->setDefaultBorder();
}

double BarChartWidget::findNewValueAxisMaxWithOutSet(QBarSet* set) const{
    double max = static_cast<QValueAxis*>(y_axis)->min();
    QList<QBarSet*> bar_sets = bar_serie->barSets();
    for(QList<QBarSet*>::const_iterator i = bar_sets.begin(); i != bar_sets.end(); ++i){
        QBarSet *current = *i;
        if(current != set)
            for(int j = 0; j < current->count(); ++j)
                if(current->at(j)*5/4 > max)
                    max = current->at(j)*5/4;
    }
    return max;
}

BarChartWidget::BarChartWidget(View *v, Model *m, QWidget *p) : XYChartWidget(new QBarCategoryAxis(), new QValueAxis(), v, m, p), bar_serie(new QBarSeries(chart)), add_new_category(new QPushButton("Add New Category")), cat_items_layout(new QFormLayout()), set_name(new QLineEdit()){
    static_cast<QBarCategoryAxis*>(x_axis)->append(""); //codice necessario per il corretto posizionamento del titolo in caso di asse senza elementi (bug qt)
    y_axis->setRange(0, 1);
    configChartWidgetItems();
    configBarChartWidgetItems();
    connectSignalsAndSlots();
    connectBarModelSignals();
    chart->addSeries(bar_serie);
    bar_serie->attachAxis(x_axis);
    bar_serie->attachAxis(y_axis);
}

void BarChartWidget::createChartFromModel(){
    XYChartWidget::createChartFromModel();
    getSetsFromModel();
    getCategoriesFromModel();
    series->setCurrentIndex(-1);
}

void BarChartWidget::removeCategory(int index){
    model->removeColumns(index+1, 1);
    if(cat_items_layout->rowCount() == 0){
        add_new_category = new QPushButton("Add New Category");
        serie_info_layout->insertRow(1, add_new_category);
        QObject::connect(add_new_category, SIGNAL(clicked(bool)), this, SLOT(userInsertFirstCategory()));
    }
}

void BarChartWidget::addCategory(int index){
    bool valid = false;
    QString cat_name;
    QBarCategoryAxis *categories_axis = static_cast<QBarCategoryAxis*>(x_axis);
    cat_name = QInputDialog::getText(this, "", "Insert new category name", QLineEdit::Normal, cat_name,  &valid);
    if(valid){
        if(static_cast<BarModel*>(model)->isCategoryNameValid(cat_name)){
            if(categories_axis->count() == 1 && categories_axis->at(0).isEmpty())
                categories_axis->remove("");
            model->insertColumns(index+1, 1);
            categories_axis->insert(index, cat_name);
            static_cast<BarModel*>(model)->changeCategoryNameAt(index, cat_name);
            valid = true;
        }
        else
            QMessageBox::warning(this, "Add Category", "Category's name must by unique");
    }
}

void BarChartWidget::changeCategoryName(int index, const QString &new_name){
    bool result = static_cast<BarModel*>(model)->changeCategoryNameAt(index, new_name);
    CategoryWidget* cat = static_cast<CategoryWidget*>(cat_items_layout->itemAt(index, QFormLayout::SpanningRole)->widget());
    if(!result){
        QMessageBox::warning(this, "Change Category Name", "Something goes wrong, rember that the name must by unique");
        cat->setBorderForTextError();
    }
    else
        cat->setDefaultBorder();
}

void BarChartWidget::changeSetValue(int index, double new_value){
    model->setData(model->index(series->currentIndex(), index+1), new_value);
}

void BarChartWidget::userInsertSet(){
    int row = series->currentIndex()+1;
    model->insertRows(row, 1);
    series->setCurrentIndex(row);
}

void BarChartWidget::userRemoveSet(){
    int row = series->currentIndex();
    if(row != -1)
        model->removeRows(series->currentIndex(), 1);
}

void BarChartWidget::userInsertFirstCategory(){
    QBarCategoryAxis *categories_axis = static_cast<QBarCategoryAxis*>(x_axis);
    addCategory(0);
    if(categories_axis->count() > 0)
        serie_info_layout->removeRow(add_new_category);

}

void BarChartWidget::userChangeSetName(){
    if(series->currentIndex() != -1){
        if(!model->setData(model->index(series->currentIndex(), 0), set_name->text())){
            QMessageBox::warning(this, "Change Set Name", "Something goes wrong, rember that the name must by unique");
            set_name->setStyleSheet("border: 1px solid red");
        }
    }
}

void BarChartWidget::multipleSetsInserted(int row, int count){
    QBarCategoryAxis *categories_axis = static_cast<QBarCategoryAxis*>(x_axis);
    if(row >= 0 && row <= bar_serie->count() && count > 0){
        QList<qreal> list;
        for(int i = 0; i < categories_axis->count(); ++i)
            list.push_back(0);
        for(int i = 0; i < count; ++i){
            QBarSet *s = new QBarSet("");
            s->setColor(model->data(model->index(row+i, 0), Qt::DecorationRole).value<QColor>());
            s->append(list);
            bar_serie->insert(row+i, s);
        }
    }
}

void BarChartWidget::multipleCategoriesInserted(int column, int count){
    for(int i = column; i < column+count; ++i){
        CategoryWidget *item = new CategoryWidget(this, i);
        cat_items_layout->insertRow(i, item);
    }
    inizializeNewCategorySetValue(column, count);
    updateIndexOfCategoryWidgetItems(column, count);
}

void BarChartWidget::multipleCategoriesRemoved(int column, int count){
    QBarCategoryAxis *categories_axis = static_cast<QBarCategoryAxis*>(x_axis);
    for(int i = column; i < column+count; ++i){
        categories_axis->remove(categories_axis->at(i));
        removeSetsValueAtIndex(i);
        cat_items_layout->removeRow(i);
    }
    cout << "Numero categories in x_axis = " << categories_axis->count();
    updateIndexOfCategoryWidgetItems(column, 0);
    if(categories_axis->count() == 0)
        categories_axis->append("");
}

void BarChartWidget::categoryAtChangedName(int index, const QString &new_name){
    CategoryWidget *item = static_cast<CategoryWidget*>(cat_items_layout->itemAt(index, QFormLayout::SpanningRole)->widget());
    QBarCategoryAxis *categories_axis = static_cast<QBarCategoryAxis*>(x_axis);
    item->setDefaultBorder();
    item->setCategoryName(new_name);
    if(index >= 0 && index < categories_axis->count() && categories_axis->at(index) != new_name){
        QString old_name = categories_axis->at(index);
        categories_axis->replace(old_name, new_name);
    }

}

void BarChartWidget::multipleSetsRemoved(int row, int count){
    if(row >= 0 && row+count-1 < bar_serie->barSets().size() && count > 0){
        for(int i = row; i < row+count; ++i){
            QBarSet *set_to_remove = bar_serie->barSets()[i];
            updateValueAxisRangeOnRemovedSet(set_to_remove);
            bar_serie->remove(set_to_remove);
        }
    }
}

void BarChartWidget::setAtChangedName(int index, const QString &new_name){
    if(index >= 0 && index < bar_serie->barSets().size())
        set_name->setStyleSheet("");
        bar_serie->barSets()[index]->setLabel(new_name);
}

void BarChartWidget::setAtChangedValue(int set_index, int value_index, double new_value){
    QBarSet *set = bar_serie->barSets()[set_index];
    value_index--;
    if(value_index >= 0 && value_index < set->count()){
        QValueAxis *y = static_cast<QValueAxis*>(y_axis);
        double old_value = set->at(value_index);
        set->replace(value_index, new_value);
        if(new_value*5/4 > y->max())
            y->setMax(new_value*5/4);
        if(old_value*5/4 == y->max())
            //se ciò è vero significa che ho diminuito il valore massimo => potrei avere un massimo più piccolo
            y->setMax(findNewValueAxisMaxWithOutSet());
    }
}

void BarChartWidget::currentSetChanged(int index){
    set_name->setText(model->data(model->index(index, 0)).toString());
    set_name->setStyleSheet("");
    updateSetValueOnSeriesCurrentIndexChanged(index);
}


void BarChartWidget::barClicked(int index, QBarSet *set){
    int set_index = 0;
    QList<QBarSet*> bars_list = bar_serie->barSets();
    for(QList<QBarSet*>::const_iterator i = bars_list.begin(); i != bars_list.end() && *i != set; ++i, ++set_index){}
    changeSetColor(set_index);
}

void BarChartWidget::changeColor(){
    int index = series->currentIndex();
    if(index != -1)
        changeSetColor(index);
}

void BarChartWidget::updateValueAxisRangeOnRemovedSet(QBarSet* set){
    QValueAxis *y = static_cast<QValueAxis*>(y_axis);
    double max = y->max();
    bool is_max_delete = false;
    for(int i = 0; i < set->count(); ++i)
        is_max_delete = (!is_max_delete) ? max/5*4 == set->at(i) : true;
    if(is_max_delete)
        y->setMax(findNewValueAxisMaxWithOutSet(set));
}




