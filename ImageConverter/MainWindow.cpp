#include "MainWindow.h"
#include <QDir>
#include <QFileDialog>
#include <QFont>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPixmap>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setupUI();
  applyStyles();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
  setWindowTitle("Image Converter");
  setMinimumSize(900, 650);

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
  headerLayout->setContentsMargins(24, 0, 24, 0);

  m_headerLabel = new QLabel("MONOCHROME");
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
  contentWidget->setObjectName("contentWidget");
  QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
  contentLayout->setContentsMargins(24, 20, 24, 20);
  contentLayout->setSpacing(16);

  // ----- Button Bar -----
  QHBoxLayout *buttonLayout = new QHBoxLayout();
  buttonLayout->setSpacing(12);

  m_uploadBtn = new QPushButton("↑ Upload");
  m_uploadBtn->setObjectName("btnUpload");
  m_convertBtn = new QPushButton("◐ Convert");
  m_convertBtn->setObjectName("btnConvert");
  m_convertBtn->setEnabled(false);
  m_saveBtn = new QPushButton("↓ Export");
  m_saveBtn->setObjectName("btnSave");
  m_saveBtn->setEnabled(false);

  connect(m_uploadBtn, &QPushButton::clicked, this, &MainWindow::onUploadImage);
  connect(m_convertBtn, &QPushButton::clicked, this,
          &MainWindow::onConvertImage);
  connect(m_saveBtn, &QPushButton::clicked, this, &MainWindow::onSaveImage);

  buttonLayout->addWidget(m_uploadBtn);
  buttonLayout->addWidget(m_convertBtn);
  buttonLayout->addWidget(m_saveBtn);
  buttonLayout->addStretch();
  contentLayout->addLayout(buttonLayout);

  // ----- Image Display Area -----
  QHBoxLayout *imageLayout = new QHBoxLayout();
  imageLayout->setSpacing(20);

  // Original Image Card
  QWidget *originalCard = new QWidget();
  originalCard->setObjectName("imageCard");
  QVBoxLayout *originalLayout = new QVBoxLayout(originalCard);
  originalLayout->setContentsMargins(16, 12, 16, 16);
  originalLayout->setSpacing(10);

  QLabel *originalTitle = new QLabel("Original Image");
  originalTitle->setObjectName("cardTitle");
  QFont titleFont = originalTitle->font();
  titleFont.setPointSize(13);
  titleFont.setBold(true);
  originalTitle->setFont(titleFont);

  m_originalImageLabel = new QLabel();
  m_originalImageLabel->setObjectName("imageLabel");
  m_originalImageLabel->setAlignment(Qt::AlignCenter);
  m_originalImageLabel->setMinimumSize(380, 380);
  m_originalImageLabel->setText("No image loaded");

  originalLayout->addWidget(originalTitle);
  originalLayout->addWidget(m_originalImageLabel, 1);

  // Converted Image Card
  QWidget *convertedCard = new QWidget();
  convertedCard->setObjectName("imageCard");
  QVBoxLayout *convertedLayout = new QVBoxLayout(convertedCard);
  convertedLayout->setContentsMargins(16, 12, 16, 16);
  convertedLayout->setSpacing(10);

  QLabel *convertedTitle = new QLabel("Black & White");
  convertedTitle->setObjectName("cardTitle");
  convertedTitle->setFont(titleFont);

  m_convertedImageLabel = new QLabel();
  m_convertedImageLabel->setObjectName("imageLabel");
  m_convertedImageLabel->setAlignment(Qt::AlignCenter);
  m_convertedImageLabel->setMinimumSize(380, 380);
  m_convertedImageLabel->setText("No conversion yet");

  convertedLayout->addWidget(convertedTitle);
  convertedLayout->addWidget(m_convertedImageLabel, 1);

  imageLayout->addWidget(originalCard);
  imageLayout->addWidget(convertedCard);
  contentLayout->addLayout(imageLayout, 1);

  mainLayout->addWidget(contentWidget);
}

void MainWindow::applyStyles() {
  QString styleSheet = R"(
        /* Main window - Linear dark theme */
        QMainWindow, QWidget {
            background-color: #0A0A0B;
            color: #E8E8ED;
        }

        /* Header - Subtle gradient border */
        #headerWidget {
            background-color: #111113;
            border-bottom: 1px solid #27272A;
        }
        #headerLabel {
            color: #8B5CF6;
        }

        /* Content area */
        #contentWidget {
            background-color: #0A0A0B;
        }

        /* Image cards - Glassmorphism style */
        #imageCard {
            background-color: #18181B;
            border: 1px solid #27272A;
            border-radius: 16px;
        }
        #imageCard:hover {
            border-color: #3F3F46;
        }
        #cardTitle {
            color: #A1A1AA;
            font-size: 12px;
            letter-spacing: 0.5px;
        }
        #imageLabel {
            background-color: #09090B;
            border: 1px solid #27272A;
            border-radius: 12px;
            color: #52525B;
            font-size: 13px;
        }

        /* Buttons - Linear style */
        QPushButton {
            background-color: #18181B;
            border: 1px solid #27272A;
            border-radius: 8px;
            padding: 10px 18px;
            color: #E8E8ED;
            font-size: 13px;
            font-weight: 500;
        }
        QPushButton:hover {
            background-color: #27272A;
            border-color: #3F3F46;
        }
        QPushButton:pressed {
            background-color: #3F3F46;
        }
        QPushButton:disabled {
            background-color: #18181B;
            color: #52525B;
            border-color: #27272A;
        }

        /* Primary button - Upload - Purple gradient */
        #btnUpload {
            background-color: #7C3AED;
            border: none;
            color: white;
        }
        #btnUpload:hover {
            background-color: #8B5CF6;
        }
        #btnUpload:pressed {
            background-color: #6D28D9;
        }

        /* Convert button - Subtle purple */
        #btnConvert {
            background-color: #18181B;
            border: 1px solid #7C3AED;
            color: #A78BFA;
        }
        #btnConvert:hover {
            background-color: #1E1B2E;
            border-color: #8B5CF6;
            color: #C4B5FD;
        }
        #btnConvert:disabled {
            background-color: #18181B;
            color: #52525B;
            border-color: #27272A;
        }

        /* Save button - Ghost style */
        #btnSave {
            background-color: transparent;
            border: 1px solid #3F3F46;
            color: #A1A1AA;
        }
        #btnSave:hover {
            background-color: #18181B;
            border-color: #52525B;
            color: #E8E8ED;
        }
        #btnSave:disabled {
            background-color: transparent;
            color: #3F3F46;
            border-color: #27272A;
        }

        /* Scrollbars - Minimal */
        QScrollBar:vertical {
            background-color: #0A0A0B;
            width: 8px;
            border-radius: 4px;
        }
        QScrollBar::handle:vertical {
            background-color: #27272A;
            border-radius: 4px;
            min-height: 30px;
        }
        QScrollBar::handle:vertical:hover {
            background-color: #7C3AED;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0;
        }
        QScrollBar:horizontal {
            background-color: #0A0A0B;
            height: 8px;
            border-radius: 4px;
        }
        QScrollBar::handle:horizontal {
            background-color: #27272A;
            border-radius: 4px;
            min-width: 30px;
        }
        QScrollBar::handle:horizontal:hover {
            background-color: #7C3AED;
        }
        QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
            width: 0;
        }

        /* Message boxes */
        QMessageBox {
            background-color: #18181B;
            color: #E8E8ED;
        }
        QMessageBox QLabel {
            color: #E8E8ED;
        }
        QMessageBox QPushButton {
            background-color: #7C3AED;
            border: none;
            color: white;
            min-width: 80px;
        }

        /* File dialog styling */
        QFileDialog {
            background-color: #18181B;
            color: #E8E8ED;
        }
        QFileDialog QListView, QFileDialog QTreeView {
            background-color: #0A0A0B;
            color: #E8E8ED;
            border: 1px solid #27272A;
            border-radius: 8px;
        }
        QFileDialog QLineEdit {
            background-color: #0A0A0B;
            color: #E8E8ED;
            border: 1px solid #27272A;
            border-radius: 6px;
            padding: 6px 10px;
        }
        QFileDialog QLineEdit:focus {
            border-color: #7C3AED;
        }
        QFileDialog QPushButton {
            background-color: #18181B;
            border: 1px solid #27272A;
            color: #E8E8ED;
        }
        QFileDialog QComboBox {
            background-color: #18181B;
            border: 1px solid #27272A;
            border-radius: 6px;
            padding: 6px 10px;
            color: #E8E8ED;
        }
        QFileDialog QComboBox:hover {
            border-color: #3F3F46;
        }
        QFileDialog QComboBox::drop-down {
            border: none;
        }
        QFileDialog QComboBox QAbstractItemView {
            background-color: #18181B;
            border: 1px solid #27272A;
            color: #E8E8ED;
            selection-background-color: #7C3AED;
        }
    )";

  setStyleSheet(styleSheet);
}

void MainWindow::displayImage(QLabel *label, const QImage &image) {
  if (image.isNull()) {
    return;
  }

  QPixmap pixmap = QPixmap::fromImage(image);
  QSize labelSize = label->size();

  // Scale the pixmap to fit the label while maintaining aspect ratio
  QPixmap scaledPixmap =
      pixmap.scaled(labelSize.width() - 20, labelSize.height() - 20,
                    Qt::KeepAspectRatio, Qt::SmoothTransformation);

  label->setPixmap(scaledPixmap);
}

void MainWindow::onUploadImage() {
  QString filePath = QFileDialog::getOpenFileName(
      this, "Select Image", QDir::homePath(),
      "Images (*.png *.jpg *.jpeg *.bmp *.gif *.webp);;All Files (*)", nullptr,
      QFileDialog::DontUseNativeDialog);

  if (filePath.isEmpty()) {
    return;
  }

  m_originalImage = QImage(filePath);

  if (m_originalImage.isNull()) {
    QMessageBox::warning(this, "Error", "Failed to load the selected image.");
    return;
  }

  displayImage(m_originalImageLabel, m_originalImage);
  m_convertBtn->setEnabled(true);

  // Clear any previous conversion
  m_convertedImage = QImage();
  m_convertedImageLabel->clear();
  m_convertedImageLabel->setText("Click 'Convert' to process");
  m_saveBtn->setEnabled(false);
}

void MainWindow::onConvertImage() {
  if (m_originalImage.isNull()) {
    return;
  }

  // Convert to grayscale (black and white)
  m_convertedImage = m_originalImage.convertToFormat(QImage::Format_Grayscale8);

  displayImage(m_convertedImageLabel, m_convertedImage);
  m_saveBtn->setEnabled(true);
}

void MainWindow::onSaveImage() {
  if (m_convertedImage.isNull()) {
    return;
  }

  QString filePath =
      QFileDialog::getSaveFileName(this, "Save Black & White Image",
                                   QDir::homePath() + "/converted_image.png",
                                   "PNG Image (*.png);;JPEG Image (*.jpg "
                                   "*.jpeg);;BMP Image (*.bmp);;All Files (*)",
                                   nullptr, QFileDialog::DontUseNativeDialog);

  if (filePath.isEmpty()) {
    return;
  }

  if (m_convertedImage.save(filePath)) {
    QMessageBox::information(this, "Success", "Image saved successfully!");
  } else {
    QMessageBox::warning(this, "Error", "Failed to save the image.");
  }
}
