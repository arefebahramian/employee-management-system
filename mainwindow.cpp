#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QMessageBox>
#include <QHeaderView>
#include <QFile>
#include <QTextStream>


Employee::Employee(QString id, QString name, QString Bakhsh, EmployeeState Vaz, QString Madrak)
    : id(id), name(name), Bakhsh(Bakhsh), Vaz(Vaz), Madrak(Madrak) {}
Employee::~Employee() {}

QString Employee::getId() const { return id; }
QString Employee::getName() const { return name; }
QString Employee::getBakhsh() const { return Bakhsh; }
EmployeeState Employee::getVaziat() const { return Vaz; }
QString Employee::getMadrak() const { return Madrak; }

void Employee::setVaz(EmployeeState newVaz)
    { Vaz = newVaz; }
void Employee::setName(QString newName)
    { name = newName; }
void Employee::setBakhsh(QString newBkhsh)
    { Bakhsh = newBkhsh; }
void Employee::setMadrak(QString newMadrak)
    { Madrak = newMadrak; }
QString Employee::getType() const
    { return "Employee"; }

QString Employee::serialize() const {
    return QString("E,%1,%2,%3,%4,%5").arg(id).arg(name).arg(Bakhsh).arg(static_cast<int>(Vaz)).arg(Madrak);
}

double Employee::calculatePerformance() const {
    double score = 50.0;
    if (Vaz == EmployeeState::Active)
        score += 25.0;
    if (Vaz == EmployeeState::Dorkar)
        score += 15.0;
    if (Madrak == "فوق لیسانس")
        score += 25.0;
    if (Madrak == "لیسانس")
        score += 15.0;
    if (score > 100)
        return 100;
    else {
        return score;
    }
}

Manager::Manager(QString id, QString name, QString Bakhsh, EmployeeState Vaz, QString Madrak)
    : Employee(id, name, Bakhsh, Vaz, Madrak) {}

QString Manager::getType() const
    { return "Manager"; }
QString Manager::serialize() const {
    return QString("M,%1,%2,%3,%4,%5").arg(getId()).arg(getName()).arg(getBakhsh()).arg(static_cast<int>(getVaziat())).arg(getMadrak());
}

double Manager::calculatePerformance() const {
    double baseScore = Employee::calculatePerformance() + 20.0;
    return (baseScore > 100.0) ? 100.0 : baseScore;
}

int Company::nextEmployeeId = 1;
Company::Company() {}
Company::~Company() { clearAll(); }
void Company::addEmployee(Employee* emp)
    { if (emp) employees.append(emp); }
void Company::removeEmployee(int index) {
    if (index >= 0 && index < employees.size()) {
        delete employees[index];
        employees.removeAt(index);
    }
}

const QVector<Employee*>& Company::getEmployees() const
    { return employees; }
void Company::clearAll()
    { qDeleteAll(employees); employees.clear(); }

void Company::saveToFile(const QString& filename) const {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;
    QTextStream out(&file);
    out.setCodec("UTF-8");
    for (const Employee* emp : employees) {
        if (emp) out << emp->serialize() << "\n";
    }
    file.close();
}


void Company::loadFromFile(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;
    clearAll();
    QTextStream in(&file);
    in.setCodec("UTF-8");
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        QStringList tokens = line.split(",");
        if (tokens.size() < 6) continue;
        QString type = tokens[0];
        QString id = tokens[1];
        QString name = tokens[2];
        QString bakhsh = tokens[3];
        EmployeeState state = static_cast<EmployeeState>(tokens[4].toInt());
        QString degree = tokens[5];
        int currentId = id.toInt();
        if (currentId >= nextEmployeeId) nextEmployeeId = currentId + 1;
        if (type == "M") addEmployee(new Manager(id, name, bakhsh, state, degree));
        else addEmployee(new Employee(id, name, bakhsh, state, degree));
    }
    file.close();
}



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Company employee management");

    setupUiStuff();

    connect(ui->AddBtn, &QPushButton::clicked, this, &MainWindow::onaddEmployee);
    connect(ui->DeletBtn, &QPushButton::clicked, this, &MainWindow::ondeleteEmployee);
    connect(ui->ErteghaBtn, &QPushButton::clicked, this, &MainWindow::onpromoteEmployee);

    if (ui->EditsaveBtn) {
        connect(ui->EditsaveBtn, &QPushButton::clicked, this, &MainWindow::onupdateEmployee);
    }

    connect(ui->Search, SIGNAL(textChanged(QString)), this, SLOT(onfilterChanged()));
    connect(ui->BoxVaz, SIGNAL(currentIndexChanged(int)), this, SLOT(onfilterChanged()));
    connect(ui->BoxBakhsh, SIGNAL(currentIndexChanged(int)), this, SLOT(onfilterChanged()));
    connect(ui->BoxMadrak, SIGNAL(currentIndexChanged(int)), this, SLOT(onfilterChanged()));

    connect(ui->ClearBtn, &QPushButton::clicked, this, &MainWindow::onclearLog);
    connect(ui->ResetBtn, &QPushButton::clicked, this, &MainWindow::onreset);

    connect(ui->MainTable, &QTableWidget::itemSelectionChanged, this, &MainWindow::onTableSelectionChanged);

    company.clearAll();
    Company::nextEmployeeId = 1001;

    for (int i = 1; i <= 32; ++i) {
        QString id = QString::number(Company::nextEmployeeId++);
        EmployeeState Vaz = static_cast<EmployeeState>(i % 5);
        QString nameSeed = QString("کارمند شماره %1").arg(id);

        QString currentBakhsh;
        if (i % 4 == 0)
            currentBakhsh = "بخش مهندسی";
        else if (i % 4 == 1)
            currentBakhsh = "بخش مالی";
        else if (i % 4 == 2)
            currentBakhsh = "بخش فروش";
        else
            currentBakhsh = "بخش اداری";

        QString currentMadrak;
        if (i % 4 == 0)
            currentMadrak = "دیپلم";
        else if (i % 4 == 1)
            currentMadrak = "فوق دیپلم";
        else if (i % 4 == 2)
            currentMadrak = "لیسانس";
        else
            currentMadrak = "فوق لیسانس";

        if (i % 6 == 0) {
            company.addEmployee(new Manager(id, nameSeed, currentBakhsh, Vaz, currentMadrak));
        } else {
            company.addEmployee(new Employee(id, nameSeed, currentBakhsh, Vaz, currentMadrak));
        }
    }
    company.saveToFile(dbFile);

    refreshTable();
    refreshDashboard();
    addLog(QString::fromUtf8("برنامه با موفقیت اجرا شد، میتوانید آغاز کنید!"));
}

MainWindow::~MainWindow()
{
    company.saveToFile(dbFile);
    delete ui;
}

void MainWindow::setupUiStuff()
{
    this->resize(1280, 760);
    this->setLayoutDirection(Qt::RightToLeft);
    ui->MainTable->setColumnCount(5);
    ui->MainTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::refreshTable()
{
    QStringList headers;
    headers << QString::fromUtf8("کد پرسنلی") << QString::fromUtf8("نام کارمند") << QString::fromUtf8("بخش") << QString::fromUtf8("تحصیلات") << QString::fromUtf8("وضعیت فعلی");
    ui->MainTable->setHorizontalHeaderLabels(headers);

    ui->MainTable->setRowCount(0);

    QString searchText = ui->Search->text().trimmed().toLower();

    bool isNumber = false;
    double targetScore = searchText.toDouble(&isNumber);

    int filterStatusIdx = ui->BoxVaz->currentIndex();
    int filterDeptIdx = ui->BoxBakhsh->currentIndex();
    int filterDegreeIdx = ui->BoxMadrak->currentIndex();

    for (Employee* emp : company.getEmployees()) {
        if (!emp) continue;

        if (!searchText.isEmpty()) {
            if (isNumber) {
                if (emp->calculatePerformance() < targetScore)
                    continue;
            } else {
                if (!emp->getName().toLower().contains(searchText) && !emp->getId().contains(searchText))
                    continue;
            }
        }

        if (filterStatusIdx > 0) {
            int targetStateIdx = -1;
            if (filterStatusIdx == 1)
                targetStateIdx = 0;
            else if (filterStatusIdx == 2)
                targetStateIdx = 1;
            else if (filterStatusIdx == 3)
                targetStateIdx = 2;
            else if (filterStatusIdx == 4)
                targetStateIdx = 3;
            else if (filterStatusIdx == 6)
                targetStateIdx = 4;

            if (targetStateIdx == -1 || static_cast<int>(emp->getVaziat()) != targetStateIdx)
                continue;
        }

        if (filterDeptIdx > 0 && emp->getBakhsh() != ui->BoxBakhsh->currentText())
            continue;

        if (filterDegreeIdx > 0 && emp->getMadrak() != ui->BoxMadrak->currentText())
            continue;

        int row = ui->MainTable->rowCount();
        ui->MainTable->insertRow(row);

        QTableWidgetItem *idItem = new QTableWidgetItem(emp->getId());
        QTableWidgetItem *nameItem = new QTableWidgetItem(emp->getName());
        QTableWidgetItem *deptItem = new QTableWidgetItem(emp->getBakhsh());
        QTableWidgetItem *degreeItem = new QTableWidgetItem(emp->getMadrak());
        QTableWidgetItem *stateItem = new QTableWidgetItem();

        if (emp->getType() == "Manager") {
            nameItem->setText(emp->getName() + QString::fromUtf8(" (مدیر)"));
            QColor managerRowBg("#ADE8F4");
            idItem->setBackground(managerRowBg); nameItem->setBackground(managerRowBg);
            deptItem->setBackground(managerRowBg); degreeItem->setBackground(managerRowBg); stateItem->setBackground(managerRowBg);
        }

        switch (emp->getVaziat()) {
            case EmployeeState::Active: stateItem->setText(QString::fromUtf8("🟢 حاضر")); stateItem->setForeground(QColor("#319795")); break;
            case EmployeeState::Morakhasi: stateItem->setText(QString::fromUtf8("🟡 مرخصی")); stateItem->setForeground(QColor("#dd6b20")); break;
            case EmployeeState::Dorkar: stateItem->setText(QString::fromUtf8("🔵 دورکار")); stateItem->setForeground(QColor("#3182ce")); break;
            case EmployeeState::PartTime: stateItem->setText(QString::fromUtf8("🟠 پاره‌وقت")); stateItem->setForeground(QColor("#805ad5")); break;
            case EmployeeState::Taligh: stateItem->setText(QString::fromUtf8("🔴 تعیلق شده")); stateItem->setForeground(QColor("#e53e3e")); break;
        }

        ui->MainTable->setItem(row, 0, idItem);
        ui->MainTable->setItem(row, 1, nameItem);
        ui->MainTable->setItem(row, 2, deptItem);
        ui->MainTable->setItem(row, 3, degreeItem);
        ui->MainTable->setItem(row, 4, stateItem);
    }
}

void MainWindow::onfilterChanged()
        { refreshTable(); refreshDashboard(); }

void MainWindow::onTableSelectionChanged()
{
    int row = ui->MainTable->currentRow();
    if (row >= 0 && ui->MainTable->item(row, 0) != nullptr) {
        QString id = ui->MainTable->item(row, 0)->text();
        for (Employee* emp : company.getEmployees()) {
            if (emp->getId() == id) {
                if (ui->EditEsm && ui->EditBakhsh && ui->EditMadrak && ui->EditVaz ) {
                    ui->EditEsm->setText(emp->getName());
                    ui->EditBakhsh->setCurrentText(emp->getBakhsh());
                    ui->EditMadrak->setCurrentText(emp->getMadrak());
                    ui->EditVaz->setCurrentIndex(static_cast<int>(emp->getVaziat()));
                }
                break;
            }
        }
    }
}

void MainWindow::onupdateEmployee()
{
    int row = ui->MainTable->currentRow();
    if (row >= 0 && ui->MainTable->item(row, 0) != nullptr) {
        QString id = ui->MainTable->item(row, 0)->text();
        if (ui->EditEsm && ui->EditBakhsh && ui->EditMadrak && ui->EditVaz) {
            for (Employee* emp : company.getEmployees()) {
                if (emp->getId() == id) {
                    emp->setName(ui->EditEsm->text());
                    emp->setBakhsh(ui->EditBakhsh->currentText());
                    emp->setMadrak(ui->EditMadrak->currentText());
                    emp->setVaz(static_cast<EmployeeState>(ui->EditVaz->currentIndex()));
                    break;
                }
            }
            company.saveToFile(dbFile);
            refreshTable();
            refreshDashboard();
            QMessageBox::information(this, QString::fromUtf8("موفقیت"), QString::fromUtf8("مشخصات کارمند ویرایش شد"));
        }
    }
}

void MainWindow::onaddEmployee()
{
    QString id = QString::number(Company::nextEmployeeId++);
    company.addEmployee(new Employee(id, QString("کارمند جدید %1").arg(id), "بخش مهندسی", EmployeeState::Active, "لیسانس"));
    company.saveToFile(dbFile);
    refreshTable(); refreshDashboard();
    addLog("کارمند جدید با موفقیت اضافه شد. شناسه: " + id);
}

void MainWindow::ondeleteEmployee()
{
    int row = ui->MainTable->currentRow();
    if (row >= 0) {
        QString id = ui->MainTable->item(row, 0)->text();
        const auto& emps = company.getEmployees();
        for(int i = 0; i < emps.size(); ++i) {
            if(emps[i]->getId() == id) { company.removeEmployee(i); break; }
        }
        company.saveToFile(dbFile);
        refreshTable(); refreshDashboard();
        addLog("کارمند از لیست حذف شد. شناسه: " + id);
    }
}

void MainWindow::onpromoteEmployee()
{
    int row = ui->MainTable->currentRow();
    if (row >= 0) {
        QString id = ui->MainTable->item(row, 0)->text();
        const auto& emps = company.getEmployees();
        for(int i = 0; i < emps.size(); ++i) {
            if(emps[i]->getId() == id && emps[i]->getType() != "Manager") {
                Employee* oldEmp = emps[i];
                company.addEmployee(new Manager(oldEmp->getId(), oldEmp->getName(), oldEmp->getBakhsh(), oldEmp->getVaziat(), oldEmp->getMadrak()));
                company.removeEmployee(i);
                break;
            }
        }
        company.saveToFile(dbFile);
        refreshTable(); refreshDashboard();
        QMessageBox::information(this, QString::fromUtf8("ارتقا"), QString::fromUtf8("نیرو به مدیر ارشد ارتقا یافت و ردیف او آبی رنگ شد!"));
    }
}

void MainWindow::onclearLog() {
    if (ui->Show) {
        ui->Show->clear();
    }
    logCount = 0;
}

void MainWindow::onreset() {

    QFile::remove(dbFile);
    company.clearAll();

    Company::nextEmployeeId = 1001;
    company.saveToFile(dbFile);

    refreshTable();
    refreshDashboard();

    addLog("لیست کامل پاکسازی شد و هیچ کارمندی وجود ندارد");
}

void MainWindow::refreshDashboard()
{
    int total = company.getEmployees().size();
    int active = 0;
    for (Employee* emp : company.getEmployees()) {
        if (emp->getVaziat() == EmployeeState::Active) active++;
    }

    ui->Amar->setText(QString::fromUtf8(" کارمند ثبت‌شده: %1 نفر و نیروهای حاضر فعال: %2 نفر").arg(total).arg(active));

    if (ui->Nesbat) {
        int percentage = (total > 0) ? (active * 100 / total) : 0;
        ui->Nesbat->setValue(percentage);
    }
}

void MainWindow::addLog(const QString &message)
{
    logCount++;
    QString timeStr = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    if (ui->Show) {
        ui->Show->append(QString("[%1] (#%2) %3").arg(timeStr).arg(logCount).arg(message));
    }
}
