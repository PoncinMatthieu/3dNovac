
/*-----------------------------------------------------------------------------

	3dNovac Core
	Copyright (C) 2010-2011, The 3dNovac Team

    This file is part of 3dNovac.

    3dNovac is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    3dNovac is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with 3dNovac.  If not, see <http://www.gnu.org/licenses/>.

    File Created At:        24/03/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/


#include <string.h>
#include "Parser.h"
#include "../Debug/CallStack.h"

using namespace std;
using namespace Nc::Utils::Xml;

Object *Parser::Parse(std::istream &in, bool parseHeader)
{
    Parser p(in, parseHeader);
    return p._content;
}

Parser::Parser(std::istream &in, bool parseHeader)
	: _in(in), _parseHeader(parseHeader)
{
    // creation d'un nouveau header/content, celui ci, stockera l'ensemble du fichier xml
    _lastObject = _content = new Object("CONTENT", Object::BLOCK);
    ReadAll();
}

void Parser::ReadAll()
{
	CALLSTACK_INFO();

    string      line;
    Object      *object;
    Token       t;

	if (_parseHeader)
	{
		// lecture de la premiere ligne, et verification du header
		GenereToken(t);
		CreateObject(t, object);
		if (object->Type() != Object::HEADER || object->Name() != "xml")
			throw Exception("Xml::Parser", "This is not an XML file");
		delete object; // pas besoin du header
	}

    // lecture du fichier si le header est bien conforme
    Read();
}

void Parser::Read()
{
    unsigned int i = 0;
    Token       token;
    Object      *newObject;

    // parcour l'ensemble du fichier
    while (GenereToken(token))
    {
      //cout << token.type << "\tNew Token Generated " << i << " with data = `" << token.data << "`" << endl;
      CreateObject(token, newObject);
      token.data.clear();
      i++;
    }
}

bool Parser::GenereToken(Token &newToken)
{
    static unsigned int     pos = 0;
	static std::streamsize	size = 0;
    static bool             endData;
    bool                    first = true;
    char                    lastCaract = ' ';

    // si on est a la fin du buffer, on le read
    // sinon, on le remplit
    endData = false;
    while (!endData)
    {
        if (first)
        {
            endData = true;
            first = false;
        }
        if (pos >= (unsigned int)size)
        {
            _in.read(_buffer, NC_UTILS_XML_SIZE_BUFFER);
            size = _in.gcount();
            pos = 0;
            if (size == 0)
                return false;
        }
        endData = SearchTokenInBuffer(newToken, pos, size, endData, lastCaract);
        lastCaract = _buffer[pos - 1];
    }
    return (pos < (unsigned int)size || !_in.eof());
}

bool Parser::SearchTokenInBuffer(Token &newToken, unsigned int &pos, const std::streamsize &size, bool lastEndData, char lastCaract)
{
    bool            endData = true;

    // search a tag, endtag or a data
    // parcour le buffer jusqu'a tombe sur un caractere

    //cout << "buffer = `";
    //for (unsigned int i = pos; i < size; i++)
      //    cout << _buffer[i];
    //cout << "`" << endl;

    // si on est deja a la fin d'un bloc de data, on peut s'arreter maintenant
    if (!lastEndData && _buffer[pos] == '<' && newToken.type == DATA)
        if (newToken.data.find_first_not_of(" \t\r\n") != string::npos)
            return true;

    // 1ere etape on selectionne le type du token
    unsigned int begin = ChooseTypeToken(newToken, pos, size, lastEndData, lastCaract);

    // 2eme etape on avance le pointeur jusqu'a la fin du tag ou de la data, et verifie si on est a la fin du buffer
    for (; pos < (unsigned int)size && (_buffer[pos] != '>' && _buffer[pos] != '<'); ++pos);
    //cout << newToken.type << " begin = " << begin << "  pos = " << pos << "  size = " << size << endl;

    // enfin si le type du token a bien ete choisi, on peut continuer a ajouter la data au token
        // ajoute la data
    if (pos > begin && newToken.type != (TOKEN_TYPE)-1)
    {
        newToken.data += string(_buffer + begin, pos - begin);
	//cout << "test type: " << newToken.type << endl;
	//cout << "test data: " << newToken.data << endl;
        if (newToken.type == DATA && newToken.data.compare(0, 9, "<![CDATA[") == 0)
            newToken.type = CDATA;
        if ((newToken.type == DATA || newToken.type == CDATA) && newToken.data.compare(0, 4, "<!--") == 0)
            newToken.type = COMMENT;
        if (newToken.type == CDATA && newToken.data.size() > 2 && newToken.data.compare(newToken.data.size() - 2, 2, "]]") == 0)
            newToken.data += '>';
        if (newToken.type == COMMENT && newToken.data.size() > 2 && newToken.data.compare(newToken.data.size() - 2, 2, "--") == 0)
            newToken.data += '>';
    }

    if (newToken.type == CDATA && newToken.data.size() > 3 && newToken.data.compare(newToken.data.size() - 3, 3, "]]>") != 0)
    {
        endData = false;
        if (pos < (unsigned int)size && (_buffer[pos] == '>' || _buffer[pos] == '<'))
        {
            newToken.data += _buffer[pos];
            pos++;
        }
    }
    if (newToken.type == COMMENT && newToken.data.size() > 3 && newToken.data.compare(newToken.data.size() - 3, 3, "-->") != 0)
    {
        endData = false;
        if (pos < (unsigned int)size && (_buffer[pos] == '>' || _buffer[pos] == '<'))
        {
            newToken.data += _buffer[pos];
            pos++;
        }
    }
    else if (pos == (unsigned int)size - 1 && _buffer[pos] == '<')
    {
        if (newToken.data.empty() || !(newToken.type == DATA && newToken.data.find_first_not_of(" \t\r\n") != string::npos))
        {
            endData = false;
            begin = ++pos;
            newToken.data.clear();
        }
    }
    else if (pos >= (unsigned int)size && newToken.type != (TOKEN_TYPE)-1)
        endData = false;
//        if (!endData)
//            cout << "endData false" << endl;

    // si on est a la fin d'un tag
    if (newToken.type != (TOKEN_TYPE)-1)
    {
        if (_buffer[pos] == '>')
        {
            if (pos > 0)
                lastCaract = _buffer[pos - 1];
            if (lastCaract != '?')
                newToken.objectType = (lastCaract == '/') ? Object::INLINE : Object::BLOCK;
            if (lastCaract == '?' || lastCaract == '/')
                newToken.data.erase(newToken.data.size() - 1);
            ++pos;
        }
        // sinon, si on est a la fin de la data, les brackets ne sont pas ferne
        else if (endData && (newToken.type == TAG || newToken.type == END_TAG))
            throw Exception("Xml::Parser", "Brackets not closed");
//        cout << "begin = " << begin << "  pos = " << pos << endl;
//        cout << "data = `" << newToken.data << "`" << endl;

    }
    return endData;    // false, si on est arrive a la fin du buffer et que l'on a pas finni de lire la data
}

unsigned int Parser::ChooseTypeToken(Token &newToken, unsigned int &pos, const std::streamsize &size, bool lastEndData, char lastCaract)
{
    unsigned int    begin = pos;

    if (lastEndData) // si on demare sur un nouveau token
    {
        newToken.type = (TOKEN_TYPE)-1;
        for (; pos < (unsigned int)size && (_buffer[pos] == ' ' || _buffer[pos] == '\t' || _buffer[pos] == '\n' || _buffer[pos] == '\r'); ++pos);
        if ((std::streamsize)pos >= size)
            return pos;
        if (_buffer[pos] != '<') // si on a pas de '<', on a un block de data
            newToken.type = DATA;
        else if (pos + 1 < (unsigned int)size && _buffer[pos] == '<' && _buffer[pos + 1] == '!')
        {
            newToken.type = CDATA;
	    begin = pos;
            ++pos;
        }
        else if (pos + 1 < (unsigned int)size && !(_buffer[pos] == ' ' || _buffer[pos] == '\t' || _buffer[pos] == '\n' || _buffer[pos] == '\r'))// sinon, si on est pas arrive en fin de buffer, on a un tag
        {
            if (_buffer[pos + 1] != '/') // tag de debut
            {
                if (_buffer[pos + 1] == '?')
                {
                    newToken.objectType = Object::HEADER;
                    ++pos;
                }
                newToken.type = TAG;
                ++pos;
            }
            else // tag de fin
            {
                newToken.type = END_TAG;
                pos += 2;
            }
            begin = pos;
        }
    }
    // sinon si le lastCaract ou le premier caract est un debut de tag, il faut construire le type du token, avant d'ajouter la data
    else if (newToken.type != CDATA && newToken.type != COMMENT && (lastCaract == '<' || _buffer[pos] == '<'))
    {
        char c = (lastCaract == '<') ? _buffer[pos] : _buffer[pos + 1];

        if (c != '/') // tag de debut
        {
            if (c == '?')
            {
                newToken.objectType = Object::HEADER;
                ++pos;
            }
            if (c == '!')
            {
                newToken.type = CDATA;
                newToken.data = "<";
            }
            else
                newToken.type = TAG;
            if (_buffer[pos] == '<')
                ++pos;
        }
        else // tag de fin
        {
            newToken.type = END_TAG;
            ++pos;
            if (_buffer[pos] == '<')
                ++pos;
        }
        begin = pos;
        if (newToken.type != CDATA)
            newToken.data.clear();
    }
    return begin;
}

void Parser::CreateObject(Token &t, Object *&newObject)
{
    string                  name;
    map<string, string>     param;

    // recup le nom et les params
    if (t.type == COMMENT)
    {
      newObject = new Object(name, Object::COMMENT, param, _lastObject);
      newObject->Data() += t.data;
      _lastObject->AddChild(newObject);
    }
    else if (t.type == TAG)
    {
        GetParamsAndName(t, name, param);
        newObject = new Object(name, t.objectType, param, _lastObject);
        if (t.objectType != Object::HEADER) // si c'est le header, on ne l'ajoute pas au content
            _lastObject->AddChild(newObject);
        if (t.objectType == Object::BLOCK)
            _lastObject = newObject;
//        cout << *newObject << endl;
    }
    else if (t.type == DATA || t.type == CDATA)
    {
        _lastObject->Type(Object::BLOCK);
        _lastObject->Data() += t.data;
        if (t.type == CDATA)
        {
            _lastObject->Data().replace(0, 9, "");
            _lastObject->CData(true);
        }
    }
    else if (_lastObject->Type() == Object::BLOCK) // sinon, on a un END_TAG, on va verifier que le c'est le bon
    {
        if (t.type == END_TAG && _lastObject->CData()) // si on a un CDATA, on enleve le ']]>'
            _lastObject->Data().replace(_lastObject->Data().size() - 3, 3, "");
        _lastObject = _lastObject->Parent();
        if (_lastObject == NULL)
            throw Exception("Xml::Parser", "Brackets not closed");
        if (_lastObject->Name() == t.data)
            throw Exception("Xml::Parser", "Brackets bad closed");
    }
    else
        throw Exception("Xml::Parser", "Brackets bad opened");
}

void Parser::GetParamsAndName(Token &token, std::string &name, map<std::string, std::string> &param)
{
    string  key, value;
    size_t  pos, tmpPos;

    pos = token.data.find_first_of(" \t\r\n");
    name = token.data.substr(0, pos);

    tmpPos = pos;
    pos = token.data.find("=", tmpPos);
    while (pos != string::npos)
    {
        // recup la cle
        key = token.data.substr(tmpPos, pos - tmpPos);
        tmpPos = key.find_last_of(" \t\r\n");
        key = key.substr(tmpPos + 1, string::npos);

        // recup la value
        value = "";
        pos = token.data.find_first_of("\"'", pos + 1) + 1;
        if (pos != string::npos && (pos - 1) > 0)
        {
            if (token.data[pos - 1] == '"')
                tmpPos = token.data.find_first_of("\"", pos);
            else
                tmpPos = token.data.find_first_of("'", pos);
            value = token.data.substr(pos, tmpPos - pos);
        }
        param[key] = value;

        pos = token.data.find("=", tmpPos);
    }
}
