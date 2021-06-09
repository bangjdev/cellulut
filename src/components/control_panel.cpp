#include "control_panel.h"
#include <iostream>
#include <string>
/*
 * Pseudo function for loading automatas
 * It should load automatas from database
 */



void ControlPanel::openNeighborsBrowser() {
    neighborsBrowser->exec();
}

void ControlPanel::openTransitionsBrowser() {
    transitionsBrowser->exec();
}

void ControlPanel::stateSettings() {

}

void ControlPanel::loadAutomatas() {
    //Init data
    try{
        DBManager dbMan = DBManager::getDB();
        dbMan.loadAutomatasFromDB();
        setAutomata(0);
        simulatorWidget->setAutomata(0);
    }
    catch(DBException e){
        QMessageBox window;
        window.setText(QString::fromStdString(e.getInfo()));
        window.exec();
    }
    automatasBrowser = new AutomatasBrowser(this);
    connect(automatasBrowser, &AutomatasBrowser::automataChanged, this, &ControlPanel::setAutomata);
    connect(automatasBrowser, &AutomatasBrowser::automataChanged, simulatorWidget, &SimulatorWidget::setAutomata);
}

void ControlPanel::loadNeighborhoods() {
//    int nbNeighbors = 4;
//    NeighborhoodStrategy** neighbors = new NeighborhoodStrategy* [nbNeighbors];
//    neighbors[0] = new VonNeumannNeighborhood();
//    neighbors[1] = new VonNeumannNeighborhoodGeneralized();
//    neighbors[2] = new MooreNeighborhood();
//    neighbors[3] = new MooreNeighborhoodGeneralized();
    std::pair<int, NeighborhoodStrategy**> loadedNeighborsInfos;
    loadedNeighborsInfos = DBManager::getDB().loadNeighborhoodFromDB();
    neighborsBrowser = new NeighborsBrowser(this);
    neighborsBrowser->setNeighborhoods(loadedNeighborsInfos.first, loadedNeighborsInfos.second);
    connect(neighborsBrowser, &NeighborsBrowser::neighborChanged, this, &ControlPanel::setNeighbor);
    connect(neighborsBrowser, &NeighborsBrowser::neighborChanged, simulatorWidget, &SimulatorWidget::setNeighbor);
}

void ControlPanel::loadTransitions() {
    int nbTransitions = AutomataManager::getAutomataManager()->getNbAutomatas();
    TransitionStrategy** transitions = new TransitionStrategy* [nbTransitions];
    for (int i = 0; i < nbTransitions; i ++) {
        transitions[i] = AutomataManager::getAutomataManager()->getAutomata(i)->getTransitionStrategy();
    }
    transitionsBrowser = new TransitionsBrowser(this);
    transitionsBrowser->setTransitions(nbTransitions, transitions);
    connect(transitionsBrowser, &TransitionsBrowser::transitionChanged, this, &ControlPanel::setTransition);
    connect(transitionsBrowser, &TransitionsBrowser::transitionChanged, simulatorWidget, &SimulatorWidget::setTransition);
}

void ControlPanel::initEventHandler() {
    connect(nbRowsSpb, SIGNAL(valueChanged(int)), simulatorWidget, SLOT(setNbRows(int)));
    connect(nbColsSpb, SIGNAL(valueChanged(int)), simulatorWidget, SLOT(setNbCols(int)));
    connect(cellSizeSpb, SIGNAL(valueChanged(int)), simulatorWidget, SLOT(setCellSize(int)));
    connect(bufferSizeSpb, SIGNAL(valueChanged(int)), simulatorWidget, SLOT(setBufferSize(int)));
    connect(sliderSpeed, SIGNAL(valueChanged(int)), simulatorWidget, SLOT(changeFrequency(int)));
    connect(btnBrowseAutomatas, &QPushButton::clicked, this, &ControlPanel::openAutomatasBrowser);
}

ControlPanel::ControlPanel(QWidget* parent, SimulatorWidget* simulatorWidget) : QWidget(parent), simulatorWidget(simulatorWidget) {
    mainLayout = new QVBoxLayout(this);

    // 1 : grid settings
    gridSettingsBox = new QGroupBox(tr("Grid settings"));
    initGridSettings();
    mainLayout->addWidget(gridSettingsBox);

    //2 : automata settings
    automataSettingsBox = new QGroupBox(tr("Automata settings"));
    initAutomataSettings();
    mainLayout->addWidget(automataSettingsBox);

    //3 : run settings
    runSettingsBox = new QGroupBox(tr("Run settings"));
    initRunSettings();
    mainLayout->addWidget(runSettingsBox);

    mainLayout->addStretch();


    loadAutomatas(); /*To replace by automata loading from database*/
    loadNeighborhoods();
    loadTransitions();

    setLayout(mainLayout);


    // Make all button disabled
    btnBrowseNeighborhoods->setDisabled(true);
    btnBrowseTransitions->setDisabled(true);
    btnEditState->setDisabled(true);

    initEventHandler();

    setMaximumWidth(500);
}

void ControlPanel::initGridSettings() {
    //Creation of the boxes widget
    nbRowsSpb = new QSpinBox(gridSettingsBox);
    nbColsSpb = new QSpinBox(gridSettingsBox);
    cellSizeSpb = new QSpinBox(gridSettingsBox);

    nbRowsSpb->setKeyboardTracking(false);
    nbRowsSpb->setValue(simulatorWidget->getNbRows());
    nbRowsSpb->setMaximum(100);
    nbRowsSpb->setMinimum(2);

    nbColsSpb->setKeyboardTracking(false);
    nbColsSpb->setValue(simulatorWidget->getNbCols());
    nbColsSpb->setMaximum(100);
    nbColsSpb->setMinimum(2);

    cellSizeSpb->setKeyboardTracking(false);
    cellSizeSpb->setValue(simulatorWidget->getCellSize());
    cellSizeSpb->setMaximum(50);
    cellSizeSpb->setMinimum(5);

    //Creation of the boxes layout
    gridSettingsLayout = new QFormLayout(gridSettingsBox);
    gridSettingsLayout->addRow("Rows number", nbRowsSpb);
    gridSettingsLayout->addRow("Cols number", nbColsSpb);
    gridSettingsLayout->addRow("Cell size", cellSizeSpb);
}

void ControlPanel::initAutomataSettings() {
    automataSettingsLayout = new QVBoxLayout(automataSettingsBox);

    //Choose automata
    automatasLabel = new QLabel("Automata", automataSettingsBox);
    btnBrowseAutomatas = new QPushButton(tr("Browse"), automataSettingsBox);
    textAutomataName = new QLineEdit(automataSettingsBox);
    automataFieldLayout = new QHBoxLayout(automataSettingsBox);
    automataFieldLayout->addWidget(automatasLabel);
    automataFieldLayout->addWidget(textAutomataName);
    automataFieldLayout->addWidget(btnBrowseAutomatas);
    automataSettingsLayout->addLayout(automataFieldLayout);

    // List states
    statesLabel = new QLabel("States", this);
    statesListWidget = new QListWidget(this);
    statesListWidget->setMaximumHeight(200);
    btnEditState = new QPushButton("Edit", this);
    statesFieldLayout = new QHBoxLayout(this);
    statesListWidget->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    statesListWidget->setMaximumHeight(100);
    statesFieldLayout->addWidget(statesLabel, 0, Qt::AlignTop);
    statesFieldLayout->addWidget(statesListWidget, 0, Qt::AlignTop);
    statesFieldLayout->addWidget(btnEditState, 0, Qt::AlignTop);
    automataSettingsLayout->addLayout(statesFieldLayout);


    //Choose neighborhood
    neighborhoodLabel = new QLabel("Neighborhood", automataSettingsBox);
    textNeighborhoodName = new QLineEdit(automataSettingsBox);
    btnBrowseNeighborhoods = new QPushButton("Browse", automataSettingsBox);
    neighborhoodFieldLayout = new QHBoxLayout(automataSettingsBox);
    connect(btnBrowseNeighborhoods, SIGNAL(clicked()), this, SLOT(openNeighborsBrowser()));

    neighborhoodFieldLayout->addWidget(neighborhoodLabel);
    neighborhoodFieldLayout->addWidget(textNeighborhoodName);
    neighborhoodFieldLayout->addWidget(btnBrowseNeighborhoods);
    automataSettingsLayout->addLayout(neighborhoodFieldLayout);

    //Choose transition rule
    transitionsLabel = new QLabel("Transition rule", automataSettingsBox);
    textTransitionName = new QLineEdit(automataSettingsBox);
    btnBrowseTransitions = new QPushButton("Browse", automataSettingsBox);
    transitionsFieldLayout = new QHBoxLayout(automataSettingsBox);
    connect(btnBrowseTransitions, SIGNAL(clicked()), this, SLOT(openTransitionsBrowser()));

    transitionsFieldLayout->addWidget(transitionsLabel);
    transitionsFieldLayout->addWidget(textTransitionName);
    transitionsFieldLayout->addWidget(btnBrowseTransitions);
    automataSettingsLayout->addLayout(transitionsFieldLayout);


}

void ControlPanel::initRunSettings() {
    runSettingsLayout = new QFormLayout(runSettingsBox);

    sliderSpeed = new BSlider(Qt::Horizontal, runSettingsBox);
    sliderSpeed->setValue(simulatorWidget->getFrequency());
    sliderSpeed->setMinimum(simulatorWidget->getFrequency());
    sliderSpeed->setMaximum(50);
    runSettingsLayout->addRow("Execution speed", sliderSpeed);
    bufferSizeSpb = new QSpinBox(runSettingsBox);
    bufferSizeSpb->setKeyboardTracking(false);
    bufferSizeSpb->setValue(simulatorWidget->getSimulator()->getBufferSize());
    bufferSizeSpb->setMaximum(500);
    bufferSizeSpb->setMinimum(2);
    runSettingsLayout->addRow("Buffer size", bufferSizeSpb);
}

ControlPanel::~ControlPanel() {
    delete gridSettingsLayout;
    delete automatasBrowser;
    delete neighborsBrowser;
}

void ControlPanel::setAutomata(int id) {
    textAutomataName->setText(QString::fromStdString(AutomataManager::getAutomataManager()->getAutomata(id)->getName()));
    textNeighborhoodName->setText(QString::fromStdString(AutomataManager::getAutomataManager()->getAutomata(id)->getNeighborhoodStrategy()->getName()));
    textTransitionName->setText(QString::fromStdString(AutomataManager::getAutomataManager()->getAutomata(id)->getTransitionStrategy()->getName()));

    btnBrowseNeighborhoods->setDisabled(false);
    btnBrowseTransitions->setDisabled(false);

    statesListWidget->clear();
    for (int i = 0; i < AutomataManager::getAutomataManager()->getAutomata(id)->getNbStates(); i ++) {
        CellState* s = AutomataManager::getAutomataManager()->getAutomata(id)->getAvailableState(i);
        std::string item = std::to_string(s->getId());
        item += ". " + s->getLabel();
        statesListWidget->addItem(item.c_str());
    }
    statesListWidget->adjustSize();
}

void ControlPanel::setNeighbor(NeighborhoodStrategy* neighbor) {
    textNeighborhoodName->setText(QString::fromStdString(neighbor->getName()));
}

void ControlPanel::setTransition(TransitionStrategy* transition) {
    textTransitionName->setText(QString::fromStdString(transition->getName()));
}

void FrequencyDisplayBox::setFrequency(int f) {
    setText(QString::number(f));
}

void ControlPanel::openAutomatasBrowser() {
    automatasBrowser->exec();
}
