#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Note.h"
#include <QListWidget>
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
  void onNoteSelected(QListWidgetItem *item);

private:
  void setupUI();
  void refreshNoteList();

  QListWidget *m_noteList;
  QTextEdit *m_noteEditor;
  std::vector<Note> m_notes;
  int m_currentNoteIndex; // -1 if no note selected
};

#endif // MAINWINDOW_H
