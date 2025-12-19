#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Note.h"
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QTextEdit>
#include <vector>

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void onNewNote();
  void onSaveNote();
  void onDeleteNote();
  void onNoteSelected(QListWidgetItem *current, QListWidgetItem *previous);
  void onSearchTextChanged(const QString &text);

private:
  void setupUI();
  void applyStyles();
  void refreshNoteList();

  QLabel *m_headerLabel;
  QLineEdit *m_searchBar;
  QListWidget *m_noteList;
  QTextEdit *m_noteEditor;
  std::vector<Note> m_notes;
  int m_currentNoteIndex; // -1 if no note selected
  QString m_searchFilter;
};

#endif // MAINWINDOW_H
