
#include <iostream>
#include <sstream>
#include "charclass.hpp"

// O banco é acessado somente pela classe objectClass.
#include "banco.hpp"
  
using namespace std;

/* * * * * START charClass: * * * * */
  
// Construtora da Classe de caracteres:
  charClass::charClass(string format) : string(format)
  {
    // Length do formato:
    int lenF = format.length();
    
    /* * * * * * * Inicio dos Testes de Validade * * * * * * */
    
    if(lenF==0) throw "String Vazia!";
    
    if(lenF==2 && format[0]=='\\')
    {
      (*this)[0] = (*this)[1];
      this->pop_back();
    }
    // Se o formato não for do tipo simples: "a" ou "b"
    else if(lenF!=1 && !(lenF==2 && format[0]=='\\'))
    // Então faça os seguintes testes de coerência:
    {
      // Caso a classe esteja vazia: "[]"
      if(lenF==2) throw "Formato Vazio: \"[]\"";
      // Caso a classe inversa esteja vazia: "[^]"
      if(lenF==3 && format[1]=='^') throw "Formato Vazio: \"[^]\"";
      // Caso a classe não termine com ']': "[abc"
      if(format[lenF-1] != ']') throw "Formato Inválido: \"[abc\"";
      // Caso o formato não comece com '[': "abc]"
      if(format[0]!='[') throw "Formato Inválido: \"abc]\"";
    
      // Verificando se é uma classe ou uma classe inversa,
      // Se for uma classe a posição de inicio é 2 se for inversa é 3.
      int j = format[1]!='^'?2:3;
      
      // Continuando com os testes:
      
      // Se o formato está no formato: "[abc]edf]"
      if(j+1<lenF)
        for( ; format[j+1]; j++)
          if(format[j] == ']') throw "Formato Inválido: \"[abc]edf]\"";
      // Note que no for acima note que o formato: []abc] ou [^]abc] é aceito.
      // Nesse caso não há erro pois o ']' será tratado como um caractere.
      
      
      // Caso o usuário escreva algo ambiguo no formato como: "[a--b]"
      j = format[1]!='^'?3:4;
      if(j+2<lenF)
        for( ; format[j+2]; j++)
          if(format[j-1]=='-' && format[j] =='-')
            throw "Formato Inválido: \"[a--b]\"";
      // Note no for acima que não há problema em escrever: "[--b]" ou "[a--]"
      
      // Caso o usuário escreva: "[a-b-c]" ou algo semelhante:
      j = format[1]!='^'?4:5;
      if(j+2<lenF)
        for( ; format[j+2]; j++)
          if(format[j-2]=='-' && format[j] =='-')
            throw "Formato Inválido: \"[a-b-c]\"";
    }
    
    /* * * * * * * Fim dos Testes de Validade * * * * * * */
  }
  
  charClass::charClass(const char* format) :
    charClass(string(format)) {}
  
// Define usado para iterar por uma classe de caracteres:
// [abc] por exemplo.
#define forClass(c,i) \
  for(i=1; c[i]!=']' || i==1; i++)

// Define usado para iterar sobre uma classe inversa de caracteres:
// [^abc]
#define forNotClass(c,i) \
  for(i=2; c[i]!=']' || i==2; i++)

// /*
bool charClass::match(string input, int pos)
{ return priv_match(input[pos], false); }
bool charClass::match(char input)
{ return priv_match(input, false); }
bool charClass::imatch(string input, int pos)
{ return priv_match(input[pos], true); }
bool charClass::imatch(char input)
{ return priv_match(input, true); }

bool charClass::priv_match(char input, bool inverted)
  {
    charClass& format = *this;
    
    // Length do formato:
    int lenF = format.length();
    
    // * * * * * * * Inicio da Execução Caso "a", "b" ou "c" * * * * * * *
    
    if(format[0] != '[' || lenF == 1)
      return input==format[0];
    
    // * * * * * * * Inicio da Execução Caso formato = "[abc]" * * * * * * *
    
    else if(format[1]!='^' && !inverted || format[1]=='^' && inverted)
    // Quando o formato for do tipo "[abc]"
    {
      int j;
      // Iterando sobre a string format
      forClass(format, j)
      {
        // Caso o format seja do tipo: "[a-z]"
        if( j != 1 && j != lenF-2 && format[j] == '-')
        {
          int ini, fim;
          if(format[j-1]<format[j+1])
            {ini = j-1; fim = j+1;}
          else
            {ini = j+1; fim = j-1;}
          
          char c=format[ini];
          for(c++ ; c < format[fim]; c++)
            if(input == c) return true;
        }
        // Caso o formato seja do tipo simples: "[abc]"
        else if( input == format[j] )
          return true;
      }
      return false;
    }
    
    // * * * * * * * Inicio da Execução Caso formato = "[^abc]" * * * * * * *
    
    else
    {
      int j;
      // Iterando sobre a string format
      forNotClass(format, j)
      {
        // Caso o format seja do tipo: "[^a-z]"
        if( j != 2 && j != lenF-2 && format[j] == '-')
        {
          int ini, fim;
          if(format[j-1]<format[j+1])
            {ini = j-1; fim = j+1;}
          else
            {ini = j+1; fim = j-1;}
          
          char c=format[ini];
          for(c++ ; c < format[fim]; c++)
            if(input == c)
              return false;
        }
        
        // Caso o format seja do tipo: "[^abc]"
        if( input == format[j])
          return false;
      }
      
      return true;
    }
    
    // * * * * * * * Fim da Execução da Função * * * * * * *
    
    // Se o fluxo de execução chegar aqui há um erro no código.
    throw "Erro no codigo! Na funcao charClass::match()!";
  }
// */

/*
 *
 * Função FindChar
 * @desc - Busca em uma string `input` por um caractere 
 *         dentre um conjunto de caracteres possíveis.
 *         
 *         Os caracteres possíveis são descritos na string `format`
 * 
 * @param - input: string a ser pesquisada.
 *        - format: string que define os caracteres à serem procurados.
 *        - pos: variável passada por referencia, contém a posição
 *               de inicio da busca no inicio da execução,
 *               e contém a posição do caractere encontrado
 *               no momento em que a execução termina.
 *
 * @return - retorna o caractere encontrado.
 *
 * @notas - o sistema aceita classes de caracteres.
 *          As classes aceitas atualmente são:
 *          
 *            "[abc]", "[a-x]", "[x-a]", "[abc-]", "[]]", "[]abc]"
 *          
 *          Também se aceita todas as inversas das classes acima:
 *
 *            "[^abc], "[^a-x]", "[^x-a]", "[^abc-]", "[^]]", "[^]abc]"
 *
 *          Por fim aceita-se também caracteres específicos:
 *
 *            "a", "b", "c" ... etc ...
 *
 *          O significado semantico de cada notação é o padrão para ERs.
 *          (foi baseado na semantica do LEX, que acredito ser padrão)
 *  
 */
char charClass::find(string input, int& pos)
{
  charClass format = *this;
  
  int len = input.length();
  int i;
  
  if(pos<0 || pos>len)
    throw "Em charClass::find() - Param 'pos' inválido";
  
  for(i=pos; input[i]; i++)
  {
    if(format.match(input[i]))
    {
      pos=i;
      return input[i];
    }
  }
  
  pos = i;
  return '\0';
}

/*
 * @nome - charClass::getClass
 *
 * @desc - essa função constroi um charClass a partir de uma
 *         string formada por uma sequencia de strings 
 *         em formatos compatíveis com um charClass
 *
 *         Ao fim da execução o inteiro pos aponta para a posição
 *         de inicio da próxima busca nessa mesma string.
 *
 *         Assim é possível fazer um parse em uma string grande
 *         transformando-a em um array de charClasse`s.
 *
 */
// /*
  charClass charClass::getClass(string format, int& pos)
  {
    int i = pos;
    
    // Tratamento dos parâmetros:
    if(pos<0 || (unsigned)pos > format.length())
      throw "Variável pos inválida! charClass::getClass";
    
    if((unsigned)pos == format.length())
      throw "String Vazia! charClass::getClass";
    
    // Caso haja um caractere de escape.
    if(format[i]=='\\')
    {
      // A resposta é o próximo caractere da string:
      charClass resp(format.substr(i+1,1));
      
      // O ponteiro pos aponta para o primeiro caractere após
      // o caractere retornado.
      pos+=2;
      
      return resp;
    }
    // Caso seja um caractere simples e não uma classe de caracteres:
    else if(format[i]!='[')
    {
      // retorne esse caractere.
      charClass resp(format.substr(i,1));
      // Aponte o ponteiro para a posição seguinte.
      pos++;
      return resp;
    }
    
    // Se chegou aqui trata-se de um formato do tipo: "[abc]" ou "[^abc]"
    
    // Agora vamos buscar o caractere de fechamento: ']'
    i+=2;
    
    // Note que i+=2 pula o primeiro caractere da classe.
    // Isso ocorre para permitir a classe "[]abc]" ou "[]]"
    
    // E para permitir as classes: "[^]abc]" e "[^]]":
    if(format[i-1]=='^') i++;
    
    if( (unsigned)i < format.length() )
    
      // Para cada caractere a partir da posição atu[abc]al.
      for(; format[i]; i++)
      {
        // Se ele for o de fechamento de couchete retorne.
        if(format[i]==']')
        {
          // resp = classe: "[...]"
          charClass resp(format.substr(pos, i-pos+1));
          // pos é atualizado para a próxima posição:
          pos = i+1;
          
          return resp;
        }
      }
      // Note que por causa do i+=2 eu pulei a primeira posição.
      // Assim garanto que eu detectarei a classe []abc]
      // E não detectarei a classe [] (essa classe não existe)
    
    // Caso o formato abra um couchete mas não o feche
    // Ele caí em caso de erro:
    throw "Formato abre couchete mas não o fecha! getClass";
  }
// */

  string charClass::str()
  {
    return (*this);
  }

/* * * * * END charClass: * * * * */


/* * * * * START strClass * * * * */

  /*
   * @nome - strClass::strClass()
   * 
   * @desc - Constroi uma lista formada por classe de caracteres.
   *         Note que é possível uma lista vazia.
   *         
   *         A interpretação format.length() para lista vazia
   *         é a palavra <lambda>
   *         ou seja, a palavra vazia.
   *         E ela é casada com qualquer string.
   *         
   */
  /*
  pMatch::strClass::strClass(string str)
  {
    int pos=0;
    
    // Tratando um erro possível:
    if(str[str.length()-1] == '\\')
      str.pop_back();
    
    while(str[pos])
      this->push_back( pMatch::charClass::getClass(str, pos) );
  }
  
  pMatch::strClass::strClass(const char* str) :
    strClass(string(str)) {}
// */
  
  /*
   * @nome - strClass::match()
   * 
   * @desc - recebe uma string e compara os primeiros caracteres
   *         dessa string com seu formato.
   *         retornando true se forem iguais.
   *
   *         note que a strClass("").match(str,pos) retorna true sempre.
   *
   */
   /*
  bool pMatch::strClass::match(string input, int pos)
  {
    // Limpa a lista antiga:
    this->match_word.clear();
    
    int len = this->size();
    strClass::iterator it = this->begin();
    
    for(int i=0; i<len; i++, it++)
    {
      if(!it->match(input[pos+i]))
        return false;
    }
    
    // Adiciona o novo item a lista e retorna:
    this->match_word.push_back(tWord(input.substr(pos,len), pos));
    return true;
  }
// */

  /*
   * @nome - strClass::find()
   * 
   * @desc - recebe uma string input, e caminha por ela até encontrar
   *         o primeiro match entre o seu formato e uma substring de input.
   *         
   *         Note que aceita-se a string vazia.        
   * 
   */
 /*
  pMatch::tWord pMatch::strClass::find(string input, int pos)
  {
    int lenF = this->size();
    
    // Se o formato for de string vazia:
    if(lenF == 0)
      // Aceite e retorne outra string vazia:
      return tWord("", pos);
    
    strClass::iterator it=this->begin();
    
    int i=pos;
    
    // Encontra o primeiro caractere do input compatível com o formato:
    (*it).find(input, i); 
    
    while(input[i])
    {
      
      if(this->match(input, i))
      {
        pos=i;
        return tWord(input.substr(i,lenF),i);
      }
      else i++;
      
      // Encontra o primeiro caractere do formato:
      (*it).find(input, i); 
    }
    
    // Caso não encontre:
    return tWord("", i);
  }
  
  pMatch::strClass pMatch::strClass::getClass(string input, int& pos)
  {
    int start = pos;
    char c;
    
    if(pos<0 || (unsigned)pos > input.length())
      throw "variável pos com valor inválido! pMatch::strClass::getClass";
     
    while(1)
    {
      // Pesquise pela string em busca de um abre parentesis:
      c = charClass("(").find(input, pos);
      
      // Se encontrou um parentesis, mas ele tinha uma barra antes:
      if(c=='(' && input[pos-1] == '\\')
      {
        // ignore-o
        pos++;
        continue;
      }
      else 
      {
        // Caso contrário retorne a string encontrada:
        return strClass(input.substr(start, pos-start));
      }
    }
  }
  
  // Gera uma string representando o conteúdo do objeto:
  string pMatch::strClass::str()
  {
    strClass::iterator it;
    string resp("");
    
    for(it = this->begin(); it!=this->end(); it++)
      resp += *it;
    return resp;
  }
// */

/* * * * * END strClass * * * * */

  
