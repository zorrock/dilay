#include <QObject>
#include <QString>
#include <algorithm>
#include <tuple>
#include <vector>
#include "view/tool/tip.hpp"

namespace {
  typedef std::tuple <ViewToolTip::MouseEvent, ViewToolTip::Modifier, QString> Tip;

  bool strictButtonOrder (ViewToolTip::MouseEvent a, ViewToolTip::MouseEvent b) {
    return static_cast <int> (a) < static_cast <int> (b);
  }

  bool strictTipOrder (const Tip& a, const Tip& b) {
    return strictButtonOrder (std::get <0> (a), std::get <0> (b));
  }

  QString tipToString (const Tip& tip) {
    QString string ("[");

    switch (std::get <1> (tip)) {
      case ViewToolTip::Modifier::None: 
        break;

      case ViewToolTip::Modifier::Ctrl: 
        string.append ("Ctrl+");
        break;

      case ViewToolTip::Modifier::Shift: 
        string.append ("Shift+");
        break;

      case ViewToolTip::Modifier::Alt: 
        string.append ("Alt+");
        break;
    }
    switch (std::get <0> (tip)) {
      case ViewToolTip::MouseEvent::Left: 
        string.append ("Left");
        break;

      case ViewToolTip::MouseEvent::Middle: 
        string.append ("Middle");
        break;

      case ViewToolTip::MouseEvent::Wheel: 
        string.append ("Wheel");
        break;

      case ViewToolTip::MouseEvent::Right: 
        string.append ("Right");
        break;
    }

    string.append ("] ");
    string.append (std::get <2> (tip));
    return string;
  }
}

struct ViewToolTip::Impl {
  typedef std::vector <Tip> Tips;

  Tips tips;

  QString toString () const {
    QString            result;
    const unsigned int numSpaces = 5;
    Tips               sorted (this->tips);
      
    std::stable_sort (sorted.begin (), sorted.end (), strictTipOrder);
    for (auto& t : sorted) {
      result.append (tipToString (t)).append (QString (" ").repeated (numSpaces));;
    }
    result.remove (result.size () - numSpaces, numSpaces);
    return result;
  }

  void add (MouseEvent event, Modifier modifier, const QString& tip) {
    this->tips.push_back (std::make_tuple (event, modifier, tip));
  }

  void add (MouseEvent event, const QString& tip) {
    this->add (event, ViewToolTip::Modifier::None, tip);
  }

  void reset () {
    this->tips.clear ();
  }
};

DELEGATE_BIG4COPY (ViewToolTip)
DELEGATE_CONST (QString, ViewToolTip, toString)
DELEGATE3      (void   , ViewToolTip, add, ViewToolTip::MouseEvent, ViewToolTip::Modifier, const QString&)
DELEGATE2      (void   , ViewToolTip, add, ViewToolTip::MouseEvent, const QString&)
DELEGATE       (void   , ViewToolTip, reset)