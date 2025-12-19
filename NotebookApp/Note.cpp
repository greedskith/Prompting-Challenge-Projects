#include "Note.h"

Note::Note(const QString &title, const QString &content)
    : m_title(title), m_content(content),
      m_lastModified(QDateTime::currentDateTime()) {}

QString Note::title() const { return m_title; }

void Note::setTitle(const QString &title) {
  m_title = title;
  m_lastModified = QDateTime::currentDateTime();
}

QString Note::content() const { return m_content; }

void Note::setContent(const QString &content) {
  m_content = content;
  m_lastModified = QDateTime::currentDateTime();
}

QDateTime Note::lastModified() const { return m_lastModified; }
