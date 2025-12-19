#include "MainWindow.h"
#include <QFont>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollBar>
#include <QSplitter>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_currentNoteIndex(-1) {
  setupUI();
  applyStyles();

  // Add some dummy data for testing
  m_notes.emplace_back("Welcome Note",
                       "This is your first note in the C++ Notebook!\n\nStart "
                       "writing your thoughts here...");
  m_notes.emplace_back(
      "Ideas", "- Learn Qt\n- Build cool apps\n- Master C++\n- Profit?");
  m_notes.emplace_back("Todo List", "☐ Finish the notebook app\n☐ Add file "
                                    "persistence\n☐ Share with friends");
  refreshNoteList();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
  setWindowTitle("Notebook");
  setMinimumSize(900, 600);

  QWidget *centralWidget = new QWidget(this);
  setCentralWidget(centralWidget);

  QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
  mainLayout->setContentsMargins(0, 0, 0, 0);
  mainLayout->setSpacing(0);

  // ===== Header =====
  QWidget *headerWidget = new QWidget();
  headerWidget->setObjectName("headerWidget");
  headerWidget->setFixedHeight(60);
  QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);
  headerLayout->setContentsMargins(20, 0, 20, 0);

  m_headerLabel = new QLabel("📓 Notebook");
  m_headerLabel->setObjectName("headerLabel");
  QFont headerFont = m_headerLabel->font();
  headerFont.setPointSize(20);
  headerFont.setBold(true);
  m_headerLabel->setFont(headerFont);

  headerLayout->addWidget(m_headerLabel);
  headerLayout->addStretch();
  mainLayout->addWidget(headerWidget);

  // ===== Content Area =====
  QWidget *contentWidget = new QWidget();
  QHBoxLayout *contentLayout = new QHBoxLayout(contentWidget);
  contentLayout->setContentsMargins(0, 0, 0, 0);
  contentLayout->setSpacing(0);

  // ----- Left Sidebar -----
  QWidget *sidebarWidget = new QWidget();
  sidebarWidget->setObjectName("sidebarWidget");
  sidebarWidget->setFixedWidth(280);
  QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebarWidget);
  sidebarLayout->setContentsMargins(12, 12, 12, 12);
  sidebarLayout->setSpacing(12);

  // Search bar
  m_searchBar = new QLineEdit();
  m_searchBar->setObjectName("searchBar");
  m_searchBar->setPlaceholderText("🔍 Search notes...");
  m_searchBar->setClearButtonEnabled(true);
  connect(m_searchBar, &QLineEdit::textChanged, this,
          &MainWindow::onSearchTextChanged);
  sidebarLayout->addWidget(m_searchBar);

  // Note list
  m_noteList = new QListWidget();
  m_noteList->setObjectName("noteList");
  m_noteList->setSpacing(4);
  connect(m_noteList, &QListWidget::currentItemChanged, this,
          &MainWindow::onNoteSelected);
  sidebarLayout->addWidget(m_noteList);

  contentLayout->addWidget(sidebarWidget);

  // ----- Right Editor Area -----
  QWidget *editorWidget = new QWidget();
  editorWidget->setObjectName("editorWidget");
  QVBoxLayout *editorLayout = new QVBoxLayout(editorWidget);
  editorLayout->setContentsMargins(20, 16, 20, 16);
  editorLayout->setSpacing(12);

  // Toolbar
  QHBoxLayout *toolbarLayout = new QHBoxLayout();
  toolbarLayout->setSpacing(10);

  QPushButton *btnNew = new QPushButton("📝 New");
  btnNew->setObjectName("btnNew");
  QPushButton *btnSave = new QPushButton("✓ Save");
  btnSave->setObjectName("btnSave");
  QPushButton *btnDelete = new QPushButton("🗑 Delete");
  btnDelete->setObjectName("btnDelete");

  connect(btnNew, &QPushButton::clicked, this, &MainWindow::onNewNote);
  connect(btnSave, &QPushButton::clicked, this, &MainWindow::onSaveNote);
  connect(btnDelete, &QPushButton::clicked, this, &MainWindow::onDeleteNote);

  toolbarLayout->addWidget(btnNew);
  toolbarLayout->addWidget(btnSave);
  toolbarLayout->addWidget(btnDelete);
  toolbarLayout->addStretch();
  editorLayout->addLayout(toolbarLayout);

  // Text editor
  m_noteEditor = new QTextEdit();
  m_noteEditor->setObjectName("noteEditor");
  m_noteEditor->setPlaceholderText("Start writing your note here...");
  QFont editorFont("SF Mono, Menlo, Monaco, monospace");
  editorFont.setPointSize(14);
  m_noteEditor->setFont(editorFont);
  editorLayout->addWidget(m_noteEditor);

  contentLayout->addWidget(editorWidget);
  mainLayout->addWidget(contentWidget);
}

void MainWindow::applyStyles() {
  QString styleSheet = R"(
    /* Main window - Nobel cream theme */
    QMainWindow, QWidget {
      background-color: #F9F8F4;
      color: #1a1a1a;
    }

    /* Header - Clean white with subtle border */
    #headerWidget {
      background-color: #FFFFFF;
      border-bottom: 1px solid #e5e7eb;
    }
    #headerLabel {
      color: #C5A059;
    }

    /* Sidebar - Light stone background */
    #sidebarWidget {
      background-color: #F5F4F0;
      border-right: 1px solid #e5e7eb;
    }

    /* Search bar - Elegant light input */
    #searchBar {
      background-color: #FFFFFF;
      border: 1px solid #d1d5db;
      border-radius: 8px;
      padding: 10px 14px;
      color: #1a1a1a;
      font-size: 13px;
    }
    #searchBar:focus {
      border: 1px solid #C5A059;
    }
    #searchBar::placeholder {
      color: #9ca3af;
    }

    /* Note list - Clean cards */
    #noteList {
      background-color: transparent;
      border: none;
      outline: none;
    }
    #noteList::item {
      background-color: #FFFFFF;
      border: 1px solid #e5e7eb;
      border-radius: 8px;
      padding: 12px;
      margin: 2px 0;
      color: #1a1a1a;
    }
    #noteList::item:hover {
      background-color: #faf9f7;
      border-color: #C5A059;
    }
    #noteList::item:selected {
      background-color: #C5A059;
      border-color: #C5A059;
      color: #FFFFFF;
    }

    /* Editor area - White canvas */
    #editorWidget {
      background-color: #FFFFFF;
    }

    /* Buttons - Elegant stone styling */
    QPushButton {
      background-color: #f3f4f6;
      border: 1px solid #d1d5db;
      border-radius: 8px;
      padding: 10px 18px;
      color: #374151;
      font-size: 13px;
      font-weight: 500;
    }
    QPushButton:hover {
      background-color: #e5e7eb;
      border-color: #9ca3af;
    }
    QPushButton:pressed {
      background-color: #d1d5db;
    }
    #btnNew {
      background-color: #C5A059;
      border: none;
      color: white;
    }
    #btnNew:hover {
      background-color: #b8944d;
    }
    #btnSave {
      background-color: #1a1a1a;
      border: none;
      color: white;
    }
    #btnSave:hover {
      background-color: #333333;
    }
    #btnDelete {
      background-color: #FFFFFF;
      border: 1px solid #fca5a5;
      color: #dc2626;
    }
    #btnDelete:hover {
      background-color: #fef2f2;
      border-color: #ef4444;
    }

    /* Note editor - Clean writing area */
    #noteEditor {
      background-color: #FFFFFF;
      border: 1px solid #e5e7eb;
      border-radius: 10px;
      padding: 16px;
      color: #1a1a1a;
      selection-background-color: #C5A059;
      selection-color: white;
    }

    /* Scrollbars - Subtle light styling */
    QScrollBar:vertical {
      background-color: #F9F8F4;
      width: 10px;
      border-radius: 5px;
    }
    QScrollBar::handle:vertical {
      background-color: #d1d5db;
      border-radius: 5px;
      min-height: 30px;
    }
    QScrollBar::handle:vertical:hover {
      background-color: #C5A059;
    }
    QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
      height: 0;
    }
    QScrollBar:horizontal {
      background-color: #F9F8F4;
      height: 10px;
      border-radius: 5px;
    }
    QScrollBar::handle:horizontal {
      background-color: #d1d5db;
      border-radius: 5px;
      min-width: 30px;
    }
    QScrollBar::handle:horizontal:hover {
      background-color: #C5A059;
    }
    QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
      width: 0;
    }
  )";

  setStyleSheet(styleSheet);
}

void MainWindow::refreshNoteList() {
  m_noteList->clear();
  for (size_t i = 0; i < m_notes.size(); ++i) {
    const auto &note = m_notes[i];

    // Filter by search
    if (!m_searchFilter.isEmpty()) {
      if (!note.title().contains(m_searchFilter, Qt::CaseInsensitive) &&
          !note.content().contains(m_searchFilter, Qt::CaseInsensitive)) {
        continue;
      }
    }

    QString displayText =
        note.title() + "\n" + note.lastModified().toString("MMM d, h:mm AP");

    QListWidgetItem *item = new QListWidgetItem(displayText);
    item->setData(Qt::UserRole, static_cast<int>(i));
    m_noteList->addItem(item);
  }
}

void MainWindow::onSearchTextChanged(const QString &text) {
  m_searchFilter = text;
  refreshNoteList();
}

void MainWindow::onNewNote() {
  m_currentNoteIndex = -1;
  m_noteEditor->clear();
  m_noteList->clearSelection();
}

void MainWindow::onSaveNote() {
  QString content = m_noteEditor->toPlainText();
  QString title = content.split('\n').first().trimmed();
  if (title.isEmpty())
    title = "Untitled Note";

  // Truncate long titles
  if (title.length() > 40) {
    title = title.left(40) + "...";
  }

  if (m_currentNoteIndex >= 0 &&
      m_currentNoteIndex < static_cast<int>(m_notes.size())) {
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
  for (int i = 0; i < m_noteList->count(); ++i) {
    if (m_noteList->item(i)->data(Qt::UserRole).toInt() == m_currentNoteIndex) {
      m_noteList->setCurrentRow(i);
      break;
    }
  }
}

void MainWindow::onDeleteNote() {
  if (m_currentNoteIndex >= 0 &&
      m_currentNoteIndex < static_cast<int>(m_notes.size())) {
    m_notes.erase(m_notes.begin() + m_currentNoteIndex);
    onNewNote();
    refreshNoteList();
  }
}

void MainWindow::onNoteSelected(QListWidgetItem *current,
                                QListWidgetItem *previous) {
  Q_UNUSED(previous);
  if (!current)
    return;

  int noteIndex = current->data(Qt::UserRole).toInt();
  if (noteIndex >= 0 && noteIndex < static_cast<int>(m_notes.size())) {
    m_currentNoteIndex = noteIndex;
    m_noteEditor->setText(m_notes[noteIndex].content());
  }
}
