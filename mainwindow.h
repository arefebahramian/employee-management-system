#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QWidget>

enum class EmployeeState {
    Active,
    Morakhasi,
    Dorkar,
    PartTime,
    Taligh
};

class Employee {
private:
    QString id;
    QString name;
    QString Bakhsh;
    EmployeeState Vaz;
    QString Madrak;

public:
    Employee(QString id, QString name, QString Bakhsh, EmployeeState Vaz, QString Madrak);
    virtual ~Employee();

    QString getId() const;
    QString getName() const;
    QString getBakhsh() const;
    EmployeeState getVaziat() const;
    QString getMadrak() const;

    void setVaz(EmployeeState newVaziat);
    void setName(QString newName);
    void setBakhsh(QString newBkhsh);
    void setMadrak(QString newMadrak);

    virtual QString getType() const;
    virtual QString serialize() const;
    virtual double calculatePerformance() const;
};

class Manager : public Employee {
public:
    Manager(QString id, QString name, QString Bakhsh, EmployeeState Vaz, QString Madrak);
    QString getType() const override;
    QString serialize() const override;
    double calculatePerformance() const override;
};

class Company {
private:
    QVector<Employee*> employees;

public:
    Company();
    ~Company();

    Company(const Company&) = delete;
    Company& operator=(const Company&) = delete;

    void addEmployee(Employee* emp);
    void removeEmployee(int index);
    const QVector<Employee*>& getEmployees() const;
    void clearAll();

    void saveToFile(const QString& filename) const;
    void loadFromFile(const QString& filename);

    static int nextEmployeeId;
};


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onaddEmployee();
    void ondeleteEmployee();
    void onpromoteEmployee();
    void onupdateEmployee();
    void onfilterChanged();
    void onTableSelectionChanged();
    void onclearLog();
    void onreset();

private:
    Ui::MainWindow *ui;
    Company company;
    QString dbFile = "company_database.txt";
    int logCount = 0;

    void setupUiStuff();
    void refreshTable();
    void refreshDashboard();
    void addLog(const QString &msg);
};

#endif // MAINWINDOW_H
