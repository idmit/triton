#ifndef CANVASACTIONS
#define CANVASACTIONS

#include <QUndoCommand>

#include "splinegroup.h"

class TypeLetterCmd : public QUndoCommand {
public:
  TypeLetterCmd(QString &text, QString letter) : text(text), letter(letter) {}
  virtual void redo() { text.append(letter); }
  virtual void undo() { text.chop(1); }

private:
  QString &text, letter;
};

class RemoveLetterCmd : public QUndoCommand {
public:
  RemoveLetterCmd(QString &text) : text(text) {}
  virtual void redo() {
    letter = text[text.size() - 1];
    text.chop(1);
  }
  virtual void undo() { text.append(letter); }

private:
  QString &text, letter;
};

#endif // CANVASACTIONS
