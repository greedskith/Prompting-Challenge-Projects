#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QImage>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void onUploadImage();
  void onConvertImage();
  void onSaveImage();

private:
  void setupUI();
  void applyStyles();
  void displayImage(QLabel *label, const QImage &image);

  QLabel *m_headerLabel;
  QLabel *m_originalImageLabel;
  QLabel *m_convertedImageLabel;
  QPushButton *m_uploadBtn;
  QPushButton *m_convertBtn;
  QPushButton *m_saveBtn;

  QImage m_originalImage;
  QImage m_convertedImage;
};

#endif // MAINWINDOW_H
