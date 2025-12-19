#include "MainWindow.h"
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QSplitter>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_currentNoteIndex(-1) {
  setupUI();

  // Add some dummy data for testing
  m_notes.emplace_back("Welcome Note",
                       "This is your first note in the C++ Notebook!");
  m_notes.emplace_back("Ideas", "- Learn Qt\n- Build cool apps\n- Profit?");
  refreshNoteList();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
  QWidget *centralWidget = new QWidget(this);
  setCentralWidget(centralWidget);

  QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

  // Left Side: Note List
  m_noteList = new QListWidget();
  connect(m_noteList, &QListWidget::itemClicked, this,
          &MainWindow::onNoteSelected);

  // Right Side: Editor
  m_noteEditor = new QTextEdit();

  // Splitter for resizable pane
  QSplitter *splitter = new QSplitter(Qt::Horizontal);
  splitter->addWidget(m_noteList);
  QWidget *rightWidget = new QWidget();
  QVBoxLayout *rightLayout = new QVBoxLayout(rightWidget);
  rightLayout->setContentsMargins(0, 0, 0, 0);

  // Toolbar / Buttons
  QHBoxLayout *buttonLayout = new QHBoxLayout();
  QPushButton *btnNew = new QPushButton("New");
  QPushButton *btnSave = new QPushButton("Save");
  QPushButton *btnDelete = new QPushButton("Delete");

  connect(btnNew, &QPushButton::clicked, this, &MainWindow::onNewNote);
  connect(btnSave, &QPushButton::clicked, this, &MainWindow::onSaveNote);
  connect(btnDelete, &QPushButton::clicked, this, &MainWindow::onDeleteNote);

  buttonLayout->addWidget(btnNew);
  buttonLayout->addWidget(btnSave);
  buttonLayout->addWidget(btnDelete);

  rightLayout->addLayout(buttonLayout);
  rightLayout->addWidget(m_noteEditor);

  splitter->addWidget(rightWidget);
  splitter->setStretchFactor(1, 1); // Give more space to editor

  mainLayout->addWidget(splitter);
}

void MainWindow::refreshNoteList() {
  m_noteList->clear();
  for (const auto &note : m_notes) {
    m_noteList->addItem(note.title());
  }
}

void MainWindow::onNewNote() {
  m_currentNoteIndex = -1;
  m_noteEditor->clear();
  m_noteList->clearSelection();
}

void MainWindow::onSaveNote() {
  QString content = m_noteEditor->toPlainText();
  QString title = content.split('\n').first();
  if (title.isEmpty())
    title = "Untitled Note";

  if (m_currentNoteIndex >= 0 && m_currentNoteIndex < m_notes.size()) {
    // Update existing
    m_notes[m_currentNoteIndex].setTitle(title);
    m_notes[m_currentNoteIndex].setContent(content);
  } else {
    // Create new
    m_notes.emplace_back(title, content);
    m_currentNoteIndex = m_notes.size() - 1;
  }
  refreshNoteList();
  // Reselect the item
  m_noteList->setCurrentRow(m_currentNoteIndex);
}

void MainWindow::onDeleteNote() {
  if (m_currentNoteIndex >= 0 && m_currentNoteIndex < m_notes.size()) {
    m_notes.erase(m_notes.begin() + m_currentNoteIndex);
    onNewNote();
    refreshNoteList();
  }
}

void MainWindow::onNoteSelected(QListWidgetItem *item) {
  int row = m_noteList->row(item);
  if (row >= 0 && row < m_notes.size()) {
    m_currentNoteIndex = row;
    m_noteEditor->setText(m_notes[row].content());
  }
}
