#include <QtWidgets/QListWidget>
#include <QtCore/QStringListModel>
#include <gui/ListViewConfig.h>
#include <SamplerGeneratorHandler/Viewer.h>
#include <SamplerGeneratorHandler/Converter.h>
#include <SampleGeneratorLib/DatasetConverters/GenericDatasetReader.h>
#include <SampleGeneratorLib/DatasetConverters/ClassTypeGeneric.h>
#include <SampleGeneratorLib/Utils/Logger.h>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QTreeView>
#include <SampleGeneratorLib/DatasetConverters/GenericDatasetWriter.h>
#include <gui/Utils.h>
#include <SamplerGeneratorHandler/Evaluator.h>
#include <SamplerGeneratorHandler/Detector.h>
#include <SampleGeneratorLib/FrameworkEvaluator/GenericInferencer.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(SampleGenerationApp* app,QWidget *parent) :
    app(app),
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupTabsInformation();




    connect(ui->pushButton, SIGNAL (released()),this, SLOT (handleViewButton()));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(setupTabsInformation()));
    connect(ui->pushButton_converter_output, SIGNAL (released()),this, SLOT (handleSelectOutputFolderButton()));
    connect(ui->pushButton_detector_output, SIGNAL (released()),this, SLOT (handleSelectOutputFolderButtonDetector()));
    connect(ui->pushButton_convert, SIGNAL (released()),this, SLOT (handleConvertButton()));
    connect(ui->pushButton_evaluate, SIGNAL (released()),this, SLOT (handleEvaluateButton()));
    connect(ui->pushButton_detect, SIGNAL (released()),this, SLOT (handleDetectButton()));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleViewButton() {
    SampleGeneratorHandler::Viewer::process(ui->listView_viewer_dataset,ui->listView_viewer_names,ui->listView_viewer_reader_imp,
                                            ui->listView_viewer_classFilter, app->getConfig()->getKey("datasetPath").getValue(), app->getConfig()->getKey("namesPath").getValue());
}

void MainWindow::handleSelectionNamesChanged() {
    std::string classNameFilePath;
    switch(ui->tabWidget->currentIndex()) {
        case 0: {
            std::vector<std::string> dataSelected;
            Utils::getListViewContent(ui->listView_viewer_names,dataSelected,app->getConfig()->getKey("namesPath").getValue() + "/");
            ClassTypeGeneric typeConverter(dataSelected[0]);
            ListViewConfig::configureInputByData(this, ui->listView_viewer_classFilter,
                                                 typeConverter.getAllAvailableClasses(), true);
            }
            break;
        case 1: {
            std::vector<std::string> dataSelected;
            Utils::getListViewContent(ui->listView_converter_names,dataSelected,app->getConfig()->getKey("namesPath").getValue() + "/");
            ClassTypeGeneric typeConverter(dataSelected[0]);
            ListViewConfig::configureInputByData(this, ui->listView_converter_classFilter,
                                                 typeConverter.getAllAvailableClasses(), true);
            }
            break;
        case 3:{
            std::vector<std::string> dataSelected;
            Utils::getListViewContent(ui->listView_evaluator_detection_names,dataSelected,app->getConfig()->getKey("namesPath").getValue() + "/");
            ClassTypeGeneric typeConverter(dataSelected[0]);
            ListViewConfig::configureInputByData(this, ui->listView_evaluator_classFilter,
                                                 typeConverter.getAllAvailableClasses(), true);
            break;
    }
        default:
            Logger::getInstance()->error("Unkown tab index");
    }
}


void MainWindow::setupTabsInformation() {
    switch(ui->tabWidget->currentIndex()) {
        case 0:
            ListViewConfig::configureDatasetInput(this, ui->listView_viewer_dataset,
                                              app->getConfig()->getKey("datasetPath").getValue(), true);
            ListViewConfig::configureInputByFile(this, ui->listView_viewer_names,
                                             app->getConfig()->getKey("namesPath").getValue(), false);
            ListViewConfig::configureInputByData(this, ui->listView_viewer_reader_imp,
                                             GenericDatasetReader::getAvailableImplementations(), false);
            connect(ui->listView_viewer_names->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(handleSelectionNamesChanged()));

            break;
        case 1:
            ListViewConfig::configureDatasetInput(this, ui->listView_converter_dataset,
                                                  app->getConfig()->getKey("datasetPath").getValue(), true);
            ListViewConfig::configureInputByFile(this, ui->listView_converter_names,
                                                 app->getConfig()->getKey("namesPath").getValue(), false);
            ListViewConfig::configureInputByData(this, ui->listView_converter_reader_imp,
                                                 GenericDatasetReader::getAvailableImplementations(), false);
            ListViewConfig::configureInputByData(this, ui->listView_converter_outImp,
                                                 GenericDatasetWriter::getAvailableImplementations(), false);

            connect(ui->listView_converter_names->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(handleSelectionNamesChanged()));

            break;
        case 2:
            ListViewConfig::configureDatasetInput(this, ui->listView_detector_dataset,
                                                  app->getConfig()->getKey("datasetPath").getValue(), true);
            ListViewConfig::configureInputByFile(this, ui->listView_detector_names,
                                                 app->getConfig()->getKey("namesPath").getValue(), false);
            ListViewConfig::configureInputByData(this, ui->listView_detector_reader_imp,
                                                 GenericDatasetReader::getAvailableImplementations(), false);
            ListViewConfig::configureInputByFile(this, ui->listView_detector_weights,
                                                 app->getConfig()->getKey("weightsPath").getValue(), false);
            ListViewConfig::configureInputByFile(this, ui->listView_detector_net_config,
                                                 app->getConfig()->getKey("netCfgPath").getValue(), false);
            ListViewConfig::configureInputByData(this, ui->listView_detector_imp,
                                                 GenericInferencer::getAvailableImplementations(), false);
            ListViewConfig::configureInputByFile(this, ui->listView_detector_names_inferencer,
                                                 app->getConfig()->getKey("namesPath").getValue(), false);

            break;
        case 3:
            ListViewConfig::configureDatasetInput(this, ui->listView_evaluator_gt_dataset,
                                                  app->getConfig()->getKey("datasetPath").getValue(), true);
            ListViewConfig::configureInputByFile(this, ui->listView_evaluator_gt_names,
                                                 app->getConfig()->getKey("namesPath").getValue(), false);
            ListViewConfig::configureInputByData(this, ui->listView_evaluator_gt_imp,
                                                 GenericDatasetReader::getAvailableImplementations(), false);
            ListViewConfig::configureDatasetInput(this, ui->listView_evaluator_dectection_dataset,
                                                  app->getConfig()->getKey("inferencesPath").getValue(), true);
            ListViewConfig::configureInputByFile(this, ui->listView_evaluator_detection_names,
                                                 app->getConfig()->getKey("namesPath").getValue(), false);
            ListViewConfig::configureInputByData(this, ui->listView_evaluator_detection_imp,
                                                 GenericDatasetReader::getAvailableImplementations(), false);
            connect(ui->listView_evaluator_detection_names->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(handleSelectionNamesChanged()));
            break;
        default:
            Logger::getInstance()->error("Unkown tab index");
    }
}

void MainWindow::handleSelectOutputFolderButton() {
    QFileDialog *fd = new QFileDialog;
    QTreeView *tree = fd->findChild <QTreeView*>();
    tree->setRootIsDecorated(true);
    tree->setItemsExpandable(false);
    fd->setFileMode(QFileDialog::Directory);
    fd->setOption(QFileDialog::ShowDirsOnly);
    fd->setViewMode(QFileDialog::Detail);
    fd->setDirectory("/mnt/large/pentalo/deep/datasets/");
    int result = fd->exec();
    QString directory;
    if (result)
    {
        directory = fd->selectedFiles()[0];
        this->ui->textEdit_converterOutPath->setText(directory);
    }
}

void MainWindow::handleConvertButton() {
    double ratio;
    ratio = this->ui->textEdit_converter_trainRatio->toPlainText().toDouble();
    std::string outputPath = this->ui->textEdit_converterOutPath->toPlainText().toStdString();
    bool splitActive = this->ui->checkBox_splitActive->isChecked();
    bool colorImage = !(this->ui->checkBox_yolo_depth->isChecked());

    SampleGeneratorHandler::Converter::process(ui->listView_converter_dataset, ui->listView_converter_names,ui->listView_converter_reader_imp,ui->listView_converter_classFilter,
            ui->listView_converter_outImp, app->getConfig()->getKey("datasetPath").getValue(),app->getConfig()->getKey("namesPath").getValue(),outputPath,splitActive,ratio, colorImage);
}

void MainWindow::handleEvaluateButton() {
    SampleGeneratorHandler::Evaluator::process(ui->listView_evaluator_gt_dataset,ui->listView_evaluator_gt_names,ui->listView_evaluator_gt_imp,
                                               ui->listView_evaluator_dectection_dataset,ui->listView_evaluator_detection_names, ui->listView_evaluator_detection_imp,
                                               ui->listView_evaluator_classFilter,app->getConfig()->getKey("datasetPath").getValue(),app->getConfig()->getKey("namesPath").getValue(),
                                               app->getConfig()->getKey("inferencesPath").getValue(),app->getConfig()->getKey("namesPath").getValue(),ui->checkBox_evaluator_merge->isChecked(),
                                               ui->checkBox_evaluator_mix->isChecked(),ui->checkBox_evaluator_show_eval->isChecked());
}

void MainWindow::handleDetectButton() {
    std::string outputPath = this->ui->textEdit_detectorOutPath->toPlainText().toStdString();
    bool useDepth = this->ui->checkBox_detector_useDepth->isChecked();
    bool singleEvaluation = this->ui->checkBox_detector_single->isChecked();


    SampleGeneratorHandler::Detector::process(ui->listView_detector_dataset, ui->listView_detector_names,ui->listView_detector_reader_imp,app->getConfig()->getKey("datasetPath").getValue(),
                                              ui->listView_detector_weights,ui->listView_detector_net_config,ui->listView_detector_imp,ui->listView_detector_names_inferencer,
                                              app->getConfig()->getKey("weightsPath").getValue(),app->getConfig()->getKey("netCfgPath").getValue(),outputPath,app->getConfig()->getKey("namesPath").getValue(),
                                              useDepth,singleEvaluation);
}

void MainWindow::handleSelectOutputFolderButtonDetector() {
    QFileDialog *fd = new QFileDialog;
    QTreeView *tree = fd->findChild <QTreeView*>();
    tree->setRootIsDecorated(true);
    tree->setItemsExpandable(false);
    fd->setFileMode(QFileDialog::Directory);
    fd->setOption(QFileDialog::ShowDirsOnly);
    fd->setViewMode(QFileDialog::Detail);
    fd->setDirectory("/mnt/large/pentalo/deep/evaluations/");
    int result = fd->exec();
    QString directory;
    if (result)
    {
        directory = fd->selectedFiles()[0];
        this->ui->textEdit_detectorOutPath->setText(directory);
    }
}


