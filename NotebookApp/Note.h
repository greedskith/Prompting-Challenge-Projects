#ifndef NOTE_H
#define NOTE_H

#include <QDateTime>
#include <QString>

class Note {
public:
  Note(const QString &title = "", const QString &content = "");

  QString title() const;
  void setTitle(const QString &title);

  QString content() const;
  void setContent(const QString &content);

  QDateTime lastModified() const;

private:
  QString m_title;
  QString m_content;
  QDateTime m_lastModified;
};

#endif // NOTE_H
