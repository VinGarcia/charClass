#include <iostream>
#include <sstream>
#include <list>
#include <vector>
#include <string>
#include <map>

#ifndef _CHARCLASS_
#define _CHARCLASS_

// Testado
class charClass : public std::string
{
  private:
  priv_match(char input, bool inverted);

  public:
  charClass(std::string format);
  charClass(const char* format);
  
  bool match(char input);
  bool match(std::string, int pos);
  bool imatch(char input);
  bool imatch(std::string, int pos);
  char find(std::string input, int& pos);
  static charClass getClass(std::string format, int& pos);
  
  std::string str();
};
  
/*
// Not going to be tested
// Considerado testado por uso estenso em outros modulos.
class matcher
{
  public:
  // Variável de escopo:
  cVar var;
  lWord match_word;
  
  public:
  // Retorna a última string encontrada pelo match();
  lWord getMatch(){return this->match_word;}
  virtual std::string str()=0;
  
  virtual bool  match   (std::string input, int pos)=0;
  //virtual tWord find    (std::string input, int pos)=0;
  //virtual tWord getClass(std::string, int pos) = 0;
  ~matcher(){}
};
  
// Testado
class strClass : public std::list<charClass>, public matcher
{
  public:
  strClass(){}
  strClass(const char* str);
  strClass(std::string str);
  
  bool match(std::string input, int pos);
  tWord find(std::string input, int pos);
  
  // A função getClass lê uma string até encontrar um '\0'
  // ou um '(' que não seja precedido por um '\\'.
  // Seu comportamento trata o '(' de forma especial.
  // As demais funções dessa classe são indiferentes ao '('.
  static strClass getClass(std::string format, int& pos);
  
  std::string str();
};
*/
  
#endif
