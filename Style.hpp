#ifndef STYLE_HPP
#define STYLE_HPP

#include <string>


namespace StyleApp {



std::string qss =   "QWidget{""background-color: rgb(239, 239, 239);""}"
                       "QTextEdit{""background-color: rgb(255, 255, 255);""border-color: rgb(0, 179, 255);""border: 5px solid rgb(0, 179, 255);""}"
                       "QPushButton{""border-radius: 00px;""background-color: rgb(239, 239, 239);""qproperty-iconSize: 28px 28px;""}"
                       "QPushButton:pressed{"" border-radius: 10px;""background-color: rgb(0, 179, 255);""}"
                       "QToolButton{""border-radius: 00px;""background-color: rgb(239, 239, 239);""qproperty-iconSize: 28px 28px;""}"
                       "QToolButton:pressed{"" border-radius: 10px;""background-color: rgb(0, 179, 255);""}";


std::string qssDark = "QWidget{""background-color: rgb(53, 53, 53);""text-color: rgb(255, 255, 255);""}"
                          "QTextEdit{""background-color: rgb(25, 25, 25);""border-color: rgb(0, 179, 255);"
                              "             ""border: 5px solid rgb(0, 179, 255);""text-color: rgb(255, 255, 255);""}"
                          "QPushButton{""border-radius: 00px;""background-color: rgb(53, 53, 53);""qproperty-iconSize: 28px 28px;""}"
                          "QPushButton:pressed{"" border-radius: 10px;""background-color: rgb(0, 179, 255);""}"
                          "QToolButton{""border-radius: 00px;""background-color: rgb(53, 53, 53);""qproperty-iconSize: 28px 28px;""}"
                          "QToolButton:pressed{"" border-radius: 10px;""background-color: rgb(0, 179, 255);""}";




}
#endif // STYLE_HPP
