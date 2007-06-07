/****************************************************************************
 **
 **
 ****************************************************************************/
#include <QtGui>

#include "BasicsMode.hh"

BasicsMode::BasicsMode(QWidget *parent, QShortcutManager *sm)
		: QWidget(parent) {
		
	setParent(0);
	mParent = parent;
	
	//here we set the default mode for when the application is executed.
	((MainWindow*)mParent)->setMode(DLFLWindow::InsertEdge);
	
	mInsertEdgeWidget = new QWidget;
  mDeleteEdgeWidget = new QWidget;
  mCollapseEdgeWidget = new QWidget;
  mSubdivideEdgeWidget = new QWidget;
  mConnectEdgesWidget = new QWidget;
  mSpliceCornersWidget = new QWidget;
  mTransformsWidget = new QWidget;

	//each mode widget will each be added to the 
	//ToolOptionsDockWidget of the MainWindow class
	setupInsertEdge();
	setupDeleteEdge();
	setupCollapseEdge();
	setupSubdivideEdge();
	setupConnectEdges();
	setupSpliceCorners();
	setupTransforms();
	
	mInsertEdgeAction = new QAction(tr("Insert Edge"),this);
	mInsertEdgeAction->setIcon(QIcon(":/images/insert_edge.png"));
	mInsertEdgeAction->setCheckable(true);
	mInsertEdgeAction->setChecked(true);
	// sm->registerAction(mInsertEdgeAction, "Basics Modes", "9");
	mInsertEdgeAction->setStatusTip(tr("Enter Insert Edge Mode"));
	mInsertEdgeAction->setToolTip(tr("Insert Edge Mode"));
	connect(mInsertEdgeAction, SIGNAL(triggered()), this, SLOT(triggerInsertEdge()));

	mDeleteEdgeAction = new QAction(tr("Delete Edge"),this);
	mDeleteEdgeAction->setIcon(QIcon(":/images/delete_edge.png"));
	mDeleteEdgeAction->setCheckable(true);
	// sm->registerAction(mDeleteEdgeAction, "Basics Modes", "9");
	mDeleteEdgeAction->setStatusTip(tr("Enter Delete Edge Mode"));
	mDeleteEdgeAction->setToolTip(tr("Delete Edge Mode"));
	connect(mDeleteEdgeAction, SIGNAL(triggered()), this, SLOT(triggerDeleteEdge()));

	mCollapseEdgeAction = new QAction(tr("Collapse Edge"),this);
	mCollapseEdgeAction->setIcon(QIcon(":/images/collapse_edge.png"));
	mCollapseEdgeAction->setCheckable(true);
	// sm->registerAction(mCollapseEdgeAction, "Basics Modes", "9");
	mCollapseEdgeAction->setStatusTip(tr("Enter Collapse Edge Mode"));
	mCollapseEdgeAction->setToolTip(tr("Collapse Edge Mode"));
	connect(mCollapseEdgeAction, SIGNAL(triggered()), this, SLOT(triggerCollapseEdge()));

	mSubdivideEdgeAction = new QAction(tr("Subdivide Edge"),this);
	mSubdivideEdgeAction->setIcon(QIcon(":/images/subdivide_edge.png"));
	mSubdivideEdgeAction->setCheckable(true);
	// sm->registerAction(mDeleteEdgeAction, "Basics Modes", "9");
	mSubdivideEdgeAction->setStatusTip(tr("Enter Subdivide Edge Mode"));
	mSubdivideEdgeAction->setToolTip(tr("Subdivide Edge Mode"));
	connect(mSubdivideEdgeAction, SIGNAL(triggered()), this, SLOT(triggerSubdivideEdge()));
	
	mConnectEdgesAction = new QAction(tr("Connect Edges"),this);
	mConnectEdgesAction->setIcon(QIcon(":/images/connect_edges.png"));
	mConnectEdgesAction->setCheckable(true);
	// sm->registerAction(mDeleteEdgeAction, "Basics Modes", "9");
	mConnectEdgesAction->setStatusTip(tr("Enter Connect Edges Mode"));
	mConnectEdgesAction->setToolTip(tr("Connect Edges Mode"));
	connect(mConnectEdgesAction, SIGNAL(triggered()), this, SLOT(triggerConnectEdges()));
	
	mSpliceCornersAction = new QAction(tr("Splice Corners"),this);
	mSpliceCornersAction->setIcon(QIcon(":/images/splice_corners.png"));
	mSpliceCornersAction->setCheckable(true);
	// sm->registerAction(mDeleteEdgeAction, "Basics Modes", "9");
	mSpliceCornersAction->setStatusTip(tr("Enter Splice Corners Mode"));
	mSpliceCornersAction->setToolTip(tr("Splice Corners Mode"));
	connect(mSpliceCornersAction, SIGNAL(triggered()), this, SLOT(triggerSpliceCorners()));

	mTransformsAction = new QAction(tr("Transforms"),this);
	mTransformsAction->setIcon(QIcon(":/images/transforms.png"));
	mTransformsAction->setCheckable(true);
	// sm->registerAction(mDeleteEdgeAction, "Basics Modes", "9");
	mTransformsAction->setStatusTip(tr("Enter Transforms Mode"));
	mTransformsAction->setToolTip(tr("Transforms Mode"));
	connect(mTransformsAction, SIGNAL(triggered()), this, SLOT(triggerTransforms()));
		
}

QMenu* BasicsMode::getMenu(){
	mBasicsMenu = new QMenu(tr("Basics"));
	
	mBasicsMenu->addAction(mInsertEdgeAction);
	mBasicsMenu->addAction(mDeleteEdgeAction);	
	mBasicsMenu->addAction(mCollapseEdgeAction);	
	mBasicsMenu->addAction(mSubdivideEdgeAction);
	mBasicsMenu->addAction(mConnectEdgesAction);	
	mBasicsMenu->addAction(mSpliceCornersAction);
	mBasicsMenu->addAction(mTransformsAction);
	
	return mBasicsMenu;
	
}

void BasicsMode::triggerInsertEdge(){
	
	((MainWindow*)mParent)->setToolOptions(mInsertEdgeWidget);
	((MainWindow*)mParent)->setMode(DLFLWindow::InsertEdge);
}

void BasicsMode::triggerDeleteEdge(){
	
	((MainWindow*)mParent)->setToolOptions(mDeleteEdgeWidget);
	((MainWindow*)mParent)->setMode(DLFLWindow::DeleteEdge);
}

void BasicsMode::triggerCollapseEdge(){
	
	((MainWindow*)mParent)->setToolOptions(mCollapseEdgeWidget);
	((MainWindow*)mParent)->setMode(DLFLWindow::CollapseEdge);
}

void BasicsMode::triggerSubdivideEdge(){
	
	((MainWindow*)mParent)->setToolOptions(mSubdivideEdgeWidget);
	((MainWindow*)mParent)->setMode(DLFLWindow::SubdivideEdge);
}

void BasicsMode::triggerConnectEdges(){
	
	((MainWindow*)mParent)->setToolOptions(mConnectEdgesWidget);
	((MainWindow*)mParent)->setMode(DLFLWindow::ConnectEdges);
}

void BasicsMode::triggerSpliceCorners(){
	
	((MainWindow*)mParent)->setToolOptions(mSpliceCornersWidget);
	((MainWindow*)mParent)->setMode(DLFLWindow::SpliceCorners);
}

void BasicsMode::triggerTransforms(){
	
	((MainWindow*)mParent)->setToolOptions(mTransformsWidget);
	((MainWindow*)mParent)->setMode(DLFLWindow::NormalMode);
}

void BasicsMode::addActions(QActionGroup *actionGroup, QToolBar *toolBar, QStackedWidget *stackedWidget){
	
	actionGroup->addAction(mInsertEdgeAction);
	actionGroup->addAction(mDeleteEdgeAction);	
	actionGroup->addAction(mCollapseEdgeAction);	
	actionGroup->addAction(mSubdivideEdgeAction);	
	actionGroup->addAction(mConnectEdgesAction);	
	actionGroup->addAction(mSpliceCornersAction);
	actionGroup->addAction(mTransformsAction);
	
	toolBar->addAction(mInsertEdgeAction);
	toolBar->addAction(mDeleteEdgeAction);	
	toolBar->addAction(mCollapseEdgeAction);	
	toolBar->addAction(mSubdivideEdgeAction);	
	toolBar->addAction(mConnectEdgesAction);	
	toolBar->addAction(mSpliceCornersAction);
	toolBar->addAction(mTransformsAction);
	
	stackedWidget->addWidget(mInsertEdgeWidget);
	stackedWidget->addWidget(mDeleteEdgeWidget);	
	stackedWidget->addWidget(mCollapseEdgeWidget);	
	stackedWidget->addWidget(mSubdivideEdgeWidget);	
	stackedWidget->addWidget(mConnectEdgesWidget);	
	stackedWidget->addWidget(mSpliceCornersWidget);
	stackedWidget->addWidget(mTransformsWidget);

}

void BasicsMode::setupInsertEdge() {
	
	mInsertEdgeLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mInsertEdgeLayout->setMargin(0);
	mInsertEdgeWidget->setWindowTitle(tr(""));
	mInsertEdgeWidget->setLayout(mInsertEdgeLayout);	
}

void BasicsMode::setupDeleteEdge() {
	
	mDeleteEdgeLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mDeleteEdgeLayout->setMargin(0);
	//cleanup checkbox
	QCheckBox *cleanupCheckBox = new QCheckBox(tr("Cleanup"),this);
	connect(cleanupCheckBox, SIGNAL(stateChanged(int)),
          ((MainWindow*)mParent), SLOT(toggleDeleteEdgeCleanupFlag(int)));
	mDeleteEdgeLayout->addWidget(cleanupCheckBox);	
	mDeleteEdgeLayout->addStretch(1);
	mDeleteEdgeWidget->setWindowTitle(tr("Delete Edge Mode"));
	mDeleteEdgeWidget->setLayout(mDeleteEdgeLayout);	
}

void BasicsMode::setupCollapseEdge() {
	
	mCollapseEdgeLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mCollapseEdgeLayout->setMargin(0);
	mCollapseEdgeWidget->setWindowTitle(tr(""));
	mCollapseEdgeWidget->setLayout(mCollapseEdgeLayout);	
}

void BasicsMode::setupSubdivideEdge() {
	
	mSubdivideEdgeLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mSubdivideEdgeLayout->setMargin(0);
	
	//number of subdivisions spinbox
	QLabel *numSubdivsLabel = new QLabel(tr("# Subdivisions"));
	// numSubdivsLabel->setAlignment(Qt::AlignRight);
	QSpinBox *numSubdivsSpinBox = new QSpinBox;
	numSubdivsSpinBox->setRange(1, 10);
	numSubdivsSpinBox->setSingleStep(1);
	numSubdivsSpinBox->setValue(0);
	numSubdivsSpinBox->setMaximumSize(50,25);
	connect(numSubdivsSpinBox, SIGNAL(valueChanged(int)),
          ((MainWindow*)mParent), SLOT(changeNumSubdivs(int)));
	
	mSubdivideEdgeLayout->addWidget(numSubdivsLabel);
	mSubdivideEdgeLayout->addWidget(numSubdivsSpinBox);
	mSubdivideEdgeLayout->addStretch(1);
	mSubdivideEdgeWidget->setWindowTitle(tr("Subdivide Edge Mode"));
	mSubdivideEdgeWidget->setLayout(mSubdivideEdgeLayout);	
	
}

void BasicsMode::setupConnectEdges(){
	
	mConnectEdgesLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mConnectEdgesLayout->setMargin(0);
	mConnectEdgesWidget->setWindowTitle(tr(""));
	mConnectEdgesWidget->setLayout(mConnectEdgesLayout);	
	
}

void BasicsMode::setupSpliceCorners(){
	
	mSpliceCornersLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mSpliceCornersLayout->setMargin(0);
	mSpliceCornersWidget->setWindowTitle(tr(""));
	mSpliceCornersWidget->setLayout(mSpliceCornersLayout);
	
}

void BasicsMode::setupTransforms(){
	
	mTransformsLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mTransformsLayout->setMargin(0);
	QLabel *transformLabel = new QLabel(tr("Translate:"));
	mTransformsLayout->addWidget(transformLabel);
	
	//x transform
	QLabel *xPosLabel = new QLabel(tr("X"));
	xPosSpinBox = new QDoubleSpinBox;
	xPosSpinBox->setRange(-100.0, 100.0);
	xPosSpinBox->setSingleStep(0.5);
	xPosSpinBox->setValue(0.0);
	xPosSpinBox->setDecimals(1);
	xPosSpinBox->setMaximumSize(50,25);
	connect(xPosSpinBox, SIGNAL(valueChanged(double)),
          ((MainWindow*)mParent), SLOT(translatex(double)));

	mTransformsLayout->addWidget(xPosLabel);
  mTransformsLayout->addWidget(xPosSpinBox);

	//y transform
	QLabel *yPosLabel = new QLabel(tr("Y"));
	yPosSpinBox = new QDoubleSpinBox;
	yPosSpinBox->setRange(-100.0, 100.0);
	yPosSpinBox->setSingleStep(0.5);
	yPosSpinBox->setValue(0.0);
	yPosSpinBox->setDecimals(1);
	yPosSpinBox->setMaximumSize(50,25);
	connect(yPosSpinBox, SIGNAL(valueChanged(double)),
          ((MainWindow*)mParent), SLOT(translatey(double)));

	mTransformsLayout->addWidget(yPosLabel);
  mTransformsLayout->addWidget(yPosSpinBox);

	//z transform
	QLabel *zPosLabel = new QLabel(tr("Z"));
	zPosSpinBox = new QDoubleSpinBox;
	zPosSpinBox->setRange(-100.0, 100.0);
	zPosSpinBox->setSingleStep(0.5);
	zPosSpinBox->setValue(0.0);
	zPosSpinBox->setDecimals(1);
	zPosSpinBox->setMaximumSize(50,25);
	connect(zPosSpinBox, SIGNAL(valueChanged(double)),
          ((MainWindow*)mParent), SLOT(translatez(double)));

	mTransformsLayout->addWidget(zPosLabel);
  mTransformsLayout->addWidget(zPosSpinBox);
	
	QLabel *scaleLabel = new QLabel(tr("Scale:"));
	mTransformsLayout->addWidget(scaleLabel);
	
	//x scale
	QLabel *xScaleLabel = new QLabel(tr("X"));
	xScaleSpinBox = new QDoubleSpinBox;
	xScaleSpinBox->setRange(0.1, 10.0);
	xScaleSpinBox->setSingleStep(0.1);
	xScaleSpinBox->setValue(1.0);
	xScaleSpinBox->setDecimals(1);	
	xScaleSpinBox->setMaximumSize(50,25);
	connect(xScaleSpinBox, SIGNAL(valueChanged(double)),
          ((MainWindow*)mParent), SLOT(scalex(double)));

	mTransformsLayout->addWidget(xScaleLabel);
  mTransformsLayout->addWidget(xScaleSpinBox);
	
	//y scale
	QLabel *yScaleLabel = new QLabel(tr("Y"));
	yScaleSpinBox = new QDoubleSpinBox;
	yScaleSpinBox->setRange(0.1, 10.0);
	yScaleSpinBox->setSingleStep(0.1);
	yScaleSpinBox->setValue(1.0);
	yScaleSpinBox->setDecimals(1);
	yScaleSpinBox->setMaximumSize(50,25);
	connect(yScaleSpinBox, SIGNAL(valueChanged(double)),
          ((MainWindow*)mParent), SLOT(scaley(double)));

	mTransformsLayout->addWidget(yScaleLabel);
  mTransformsLayout->addWidget(yScaleSpinBox);

	//z scale
	QLabel *zScaleLabel = new QLabel(tr("Z"));
	zScaleSpinBox = new QDoubleSpinBox;
	zScaleSpinBox->setRange(0.1, 10.0);
	zScaleSpinBox->setSingleStep(0.1);
	zScaleSpinBox->setValue(1.0);
	zScaleSpinBox->setDecimals(1);
	zScaleSpinBox->setMaximumSize(50,25);
	connect(zScaleSpinBox, SIGNAL(valueChanged(double)),
          ((MainWindow*)mParent), SLOT(scalez(double)));
	mTransformsLayout->addWidget(zScaleLabel);
  mTransformsLayout->addWidget(zScaleSpinBox);

	QPushButton *freezeTransformsButton = new QPushButton(tr("&Freeze Transforms"));
	connect(freezeTransformsButton, SIGNAL(clicked()),
          this, SLOT(freeze_transforms()));
	
	mTransformsLayout->addWidget(freezeTransformsButton);	
	mTransformsLayout->addStretch(1);
	mTransformsWidget->setWindowTitle(tr("Transforms Mode"));
	mTransformsWidget->setLayout(mTransformsLayout);	
}

void BasicsMode::freeze_transforms()
{
	((MainWindow*)mParent)->freeze_transforms();
	xPosSpinBox->setValue(0.0);
	yPosSpinBox->setValue(0.0);
	zPosSpinBox->setValue(0.0);
	xScaleSpinBox->setValue(1.0);
	yScaleSpinBox->setValue(1.0);
	zScaleSpinBox->setValue(1.0);
}
