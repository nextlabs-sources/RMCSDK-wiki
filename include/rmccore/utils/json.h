//
//  json.hpp
//  rmccore
//
//  Created by Gavin Ye on 10/19/17.
//  Copyright © 2017 NextLabs, Inc. All rights reserved.
//

#pragma once
#ifndef __RMCCORE_UTILS_JSON_H__
#define __RMCCORE_UTILS_JSON_H__

#include <cstdint>
#include <string>
#include <memory>
#include <vector>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <exception>

#include "rmccore/base/coreerror.h"
#include "rmccore/base/coredefs.h"
#include "rmccore/utils/string.h"

namespace RMCCORE {
    

typedef enum JsonValueType {
    JNULL = 0,
    JBOOL,
    JNUMBER,
    JSTRING,
    JOBJECT,
    JARRAY
} JsonValueType;

// Forward
class JsonNull;
class JsonBool;
class JsonNumber;
class JsonString;
class JsonObject;
class JsonArray;
class JsonParser;

class JsonValue
{
public:
    virtual ~JsonValue() { clear(); }
    
    static JsonNull* CreateNull();
    static JsonBool* CreateBool(bool v);
    static JsonNumber* CreateNumber(int32_t v);
    static JsonNumber* CreateNumber(int64_t v);
    static JsonNumber* CreateNumber(uint32_t v);
    static JsonNumber* CreateNumber(uint64_t v);
    static JsonNumber* CreateNumber(float v);
    static JsonNumber* CreateNumber(double v);
    static JsonString* CreateString(const std::string& v);
    static JsonObject* CreateObject();
    static JsonArray* CreateArray();
    
    inline JsonValueType GetType() const { return _type; }
    inline bool IsNull() const { return (JNULL == _type); }
    inline bool IsBool() const { return (JBOOL == _type); }
    inline bool IsNumber() const { return (JNUMBER == _type); }
    inline bool IsString() const { return (JSTRING == _type); }
    inline bool IsObject() const { return (JOBJECT == _type); }
    inline bool IsArray() const { return (JARRAY == _type); }
    
    virtual JsonNull* AsNull() { return nullptr; }
    virtual const JsonNull* AsNull() const { return nullptr; }
    virtual JsonBool* AsBool() { return nullptr; }
    virtual const JsonBool* AsBool() const { return nullptr; }
    virtual JsonNumber* AsNumber() { return nullptr; }
    virtual const JsonNumber* AsNumber() const { return nullptr; }
    virtual JsonString* AsString() { return nullptr; }
    virtual const JsonString* AsString() const { return nullptr; }
    virtual JsonObject* AsObject() { return nullptr; }
    virtual const JsonObject* AsObject() const { return nullptr; }
    virtual JsonArray* AsArray() { return nullptr; }
    virtual const JsonArray* AsArray() const { return nullptr; }
    
    virtual void clear() {}
    virtual bool empty() const { return true; }
    
protected:
    JsonValue() : _type(JNULL) {}
    JsonValue(JsonValueType type) : _type(type) {}
    
private:
    // No copy/move allowed
    JsonValue(const JsonValue& rhs) : _type(JNULL) {}
    JsonValue(JsonValue&& rhs) : _type(JNULL) {}
    
private:
    JsonValueType   _type;
};


inline bool IsValidJsonValue(const JsonValue* p) { return (p && !p->IsNull()); }
inline bool IsValidJsonBool(const JsonValue* p) { return (p && p->IsBool()); }
inline bool IsValidJsonNumber(const JsonValue* p) { return (p && p->IsNumber()); }
inline bool IsValidJsonString(const JsonValue* p) { return (p && p->IsString()); }
inline bool IsValidJsonObject(const JsonValue* p) { return (p && p->IsObject()); }
inline bool IsValidJsonArray(const JsonValue* p) { return (p && p->IsArray()); }

typedef JsonValue*          PJSONVALUE;
typedef const JsonValue*    PCJSONVALUE;
typedef JsonNull*           PJSONNULL;
typedef const JsonNull*     PCJSONNULL;
typedef JsonBool*           PJSONBOOL;
typedef const JsonBool*     PCJSONBOOL;
typedef JsonNumber*         PJSONNUMBER;
typedef const JsonNumber*   PCJSONNUMBER;
typedef JsonString*         PJSONSTRING;
typedef const JsonString*   PCJSONSTRING;
typedef JsonObject*         PJSONOBJECT;
typedef const JsonObject*   PCJSONOBJECT;
typedef JsonArray*          PJSONARRAY;
typedef const JsonArray*    PCJSONARRAY;

class JsonNull : public JsonValue
{
public:
    virtual ~JsonNull() {}
    virtual JsonNull* AsNull() { return dynamic_cast<JsonNull*>(this); }
    virtual const JsonNull* AsNull() const { return dynamic_cast<const JsonNull*>(this); }
    
protected:
    JsonNull() : JsonValue(JNULL) {}
    friend class JsonValue;
};

class JsonBool : public JsonValue
{
public:
    virtual ~JsonBool() {}
    
    virtual JsonBool* AsBool() { return this; }
    virtual const JsonBool* AsBool() const { return this; }
    
    virtual void clear() {}
    virtual bool empty() const { return false; }
    
    inline bool GetBool() const { return _v; }
    
protected:
    JsonBool() : JsonValue(JBOOL), _v(false)
    {
    }
    
    JsonBool(bool b) : JsonValue(JBOOL), _v(b)
    {
    }
    
private:
    bool _v;
    
    friend class JsonValue;
};

class JsonNumber : public JsonValue
{
public:
    virtual ~JsonNumber() {}
    
    virtual JsonNumber* AsNumber() { return this; }
    virtual const JsonNumber* AsNumber() const { return this; }
    
    virtual void clear() { _v.n = 0; }
    virtual bool empty() const { return false; }
    
    bool IsDecimal() const { return _decimal; }
	bool IsUnsigned() const { return _unsigned; }
	bool Is64() const { return _i64; }
    int32_t ToInt() const
    {
                    return static_cast<int>(ToInt64());
    }
    int64_t ToInt64() const
    {
        if (IsDecimal()) {
            return static_cast<int64_t>(_v.f);
        }
        else {
            return _v.n;
        }
    }
    uint32_t ToUint() const
    {
        return (uint32_t)ToUint64();
    }
    uint64_t ToUint64() const
    {
        return _v.u;
    }
    double ToFloat() const
    {
        if (IsDecimal()) {
            return _v.f;
        }
        else {
            return static_cast<double>(_v.n);
        }
    }

protected:
    JsonNumber() : JsonValue(JNUMBER), _v({ 0 }), _decimal(false), _unsigned(true), _i64(true)
    {
    }
    
    JsonNumber(int n) : JsonValue(JNUMBER), _v({ 0 }), _decimal(false), _unsigned(false), _i64(false)
    {
        _v.n = static_cast<int64_t>(n);
    }
    
    JsonNumber(int64_t n) : JsonValue(JNUMBER), _v({ 0 }), _decimal(false), _unsigned(false), _i64(true)
    {
        _v.n = n;
    }
    
    JsonNumber(unsigned int n) : JsonValue(JNUMBER), _v({ 0 }), _decimal(false), _unsigned(true), _i64(false)
    {
        memcpy(&_v.n, &n, sizeof(unsigned int));
    }
    
    JsonNumber(uint64_t n) : JsonValue(JNUMBER), _v({ 0 }), _decimal(false), _unsigned(true), _i64(true)
    {
        _v.u = n;
    }
    
    JsonNumber(float f) : JsonValue(JNUMBER), _v({ 0 }), _decimal(true), _unsigned(false), _i64(false)
    {
        _v.f = f;
    }
    
    JsonNumber(double f) : JsonValue(JNUMBER), _v({ 0 }), _decimal(true), _unsigned(false), _i64(false)
    {
        _v.f = f;
    }
    
private:
    bool _decimal;
	bool _unsigned;
	bool _i64;
    union {
        int64_t n;
		uint64_t u;
        double  f;
    }   _v;
    
    friend class JsonValue;
};

class JsonString : public JsonValue
{
public:
    virtual ~JsonString() {}
    
    virtual JsonString* AsString() { return this; }
    virtual const JsonString* AsString() const { return this; }
    
    virtual void clear() { _v.clear(); }
    virtual bool empty() const { return _v.empty(); }
    
    const std::string& GetString() const { return _v; }
    
protected:
    JsonString() : JsonValue(JSTRING) {}
    JsonString(const std::string& s) : JsonValue(JSTRING), _v(s) {}
    
private:
    std::string    _v;
    
    friend class JsonValue;
};

class JsonObject : public JsonValue
{
public:
    virtual ~JsonObject()
    {
        clear();
    }
    
    typedef std::vector<std::pair<std::string, JsonValue*>>::iterator        iterator;
    typedef std::vector<std::pair<std::string, JsonValue*>>::const_iterator  const_iterator;
    typedef std::pair<std::string, JsonValue*>                             value_type;
    
    virtual JsonObject* AsObject() { return this; }
    virtual const JsonObject* AsObject() const { return this; }
    
    inline iterator begin() { return _v.begin(); }
    inline iterator end() { return _v.end(); }
    inline const_iterator cbegin() const { return _v.cbegin(); }
    inline const_iterator cend()const { return _v.cend(); }
    
    virtual void clear()
    {
        while (!_v.empty()) {
            auto it = _v.begin();
            if (NULL != (*it).second) {
                delete (*it).second;
                (*it).second = NULL;
            }
            _v.erase(it);
        }
    }
    virtual bool empty() const { return _v.empty(); }
   
    size_t size() const { return _v.size(); }
    
    JsonValue* at(const std::string& key)
    {
        auto it = std::find_if(_v.begin(), _v.end(), [&](const value_type& it) -> bool {
            return keyCompare(it, key);
        });
        return (it == _v.end()) ? nullptr : it->second;
    }
    
    const JsonValue* at(const std::string& key) const
    {
        auto it = std::find_if(_v.begin(), _v.end(), [&](const value_type& it) -> bool {
            return keyCompare(it, key);
        });
        return (it == _v.end()) ? nullptr : it->second;
    }
    
    bool set(const std::string& key, JsonValue* v)
    {
        if (key.empty() || v == nullptr)
            return false;
        
        auto it = std::find_if(_v.begin(), _v.end(), [&](const value_type& it) -> bool {
            return keyCompare(it, key);
        });
        
        if (it == _v.end()) {
            _v.push_back(std::pair<std::string,JsonValue*>(key, v));
        }
        else {
            if (it->second) {
                delete it->second;
                it->second = nullptr;
            }
            it->second = v;
        }
        
        return true;
    }
    
    JsonValue* remove(const std::string& key)
    {
        JsonValue* p = nullptr;
        auto it = std::find_if(_v.begin(), _v.end(), [&](const value_type& it) -> bool {
            return keyCompare(it, key);
        });
        if (it != _v.end()) {
            p = it->second;
            it->second = nullptr;
        }
        _v.erase(it);
        return p;
    }
    
    bool erase(const std::string& key)
    {
        bool erased = false;
        auto it = std::find_if(_v.begin(), _v.end(), [&](const value_type& it) -> bool {
            return keyCompare(it, key);
        });
        if (it != _v.end()) {
            if (it->second) {
                delete it->second;
                it->second = nullptr;
            }
            _v.erase(it);
            erased = true;
        }
        return erased;
    }
    
protected:
    JsonObject() : JsonValue(JOBJECT) {}
    
    bool keyCompare(const value_type& it, const std::string& key) const
    {
        if(it.first.length() != key.length())
            return false;
        for(int i=0; i<(int)key.length(); i++)
        {
            if(it.first.c_str()[i] != key.c_str()[i])
                return false;
        }
        return true;
    }
    
    
private:
    std::vector<std::pair<std::string, JsonValue*>> _v;
    
    friend class JsonValue;
};

class JsonArray : public JsonValue
{
public:
    virtual ~JsonArray()
    {
        clear();
    }
    
    typedef std::vector<JsonValue*>::iterator       iterator;
    typedef std::vector<JsonValue*>::const_iterator const_iterator;
    typedef JsonValue*                              value_type;
    
    iterator begin() { return _v.begin(); }
    iterator end() { return _v.end(); }
    const_iterator cbegin() const { return _v.cbegin(); }
    const_iterator cend() const { return _v.cend(); }
    
    virtual JsonArray* AsArray() { return this; }
    virtual const JsonArray* AsArray() const { return this; }
    
    virtual void clear()
    {
        for (int i = 0; i < (int)_v.size(); i++)
        {
            JsonValue* p = _v[i];
            if (nullptr != p) {
                delete p;
                _v[i] = nullptr;
            }
        }
        _v.clear();
    }
    virtual bool empty() const { return _v.empty(); }
    
    size_t size() const { return _v.size(); }
    
    JsonValue* at(size_t i)
    {
        if (i < _v.size())
            return _v[i];
        else
            return nullptr;
    }
    
    const JsonValue* at(size_t i) const
    {
        if (i < _v.size())
            return _v[i];
        else
            return nullptr;
    }
    
    bool push_back(JsonValue* v)
    {
        if (v == nullptr)
            return false;
        _v.push_back(v);
        return true;
    }
    
    JsonValue* pop_back()
    {
        JsonValue* p = nullptr;
        if (!_v.empty()) {
            p = _v[_v.size() - 1];
            _v[_v.size() - 1] = nullptr;
            _v.pop_back();
        }
        return p;
    }
    
    bool erase(size_t i)
    {
        bool erased = false;
        if (i < _v.size()) {
            JsonValue* p = _v[i];
            if (p) {
                _v[i] = nullptr;
                delete p;
            }
            _v.erase(_v.begin() + i);
            erased = true;
        }
        return erased;
    }
    
protected:
    JsonArray() : JsonValue(JARRAY) {}
    
private:
    std::vector<JsonValue*> _v;
    
    friend class JsonValue;
};
            
typedef enum WriterType {
    Compacted = 0,
    Formatted
} WriterType;
    
class Writer
{
public:
    Writer() : _indent(0), _type(Compacted) {}
    virtual ~Writer() {}
                
protected:
    void WriteValue(std::basic_ostream<char>& os, const JsonValue* v)
    {
        if (v->IsNull()) {
            WriteNull(os, v->AsNull());
        }
        else if (v->IsBool()) {
            WriteBool(os, v->AsBool());
        }
        else if (v->IsNumber()) {
            WriteNumber(os, v->AsNumber());
        }
        else if (v->IsString()) {
            WriteString(os, v->AsString());
        }
        else if (v->IsObject()) {
            WriteObject(os, v->AsObject());
        }
        else if (v->IsArray()) {
            WriteArray(os, v->AsArray());
        }
        else {
            ;
        }
    }
    
    inline bool IsFormatted() const { return (WriterType::Formatted == _type); }
    
protected:
    
    inline void SetType(WriterType type) { _type = type; }
    inline void IncreaseIndent() { _indent += 4; }
    inline void DecreaseIndent() { _indent = (_indent > 4) ? (_indent - 4) : 0; }
    inline void WriteIndent(std::basic_ostream<char>& os)
    {
        std::vector<char> indentbuf;
        indentbuf.resize(_indent, ' ');
        os << std::basic_string<char>(indentbuf.begin(), indentbuf.end());
    }
    inline void WriteLineEnd(std::basic_ostream<char>& os)
    {
        //#ifdef _WIN32
        //                os << ((T)'\r');
        //#endif
        os << ('\n');
    }
    
    void WriteNull(std::basic_ostream<char>& os, const JsonNull* v)
    {
        static const std::string name("null");
        os << std::basic_string<char>(name.begin(), name.end());
    }
    
    void WriteBool(std::basic_ostream<char>& os, const JsonBool* v)
    {
        static const std::string sTrue("true");
        static const std::string sFalse("false");
        os << (v->GetBool() ? sTrue : sFalse);
    }
    
    void WriteNumber(std::basic_ostream<char>& os, const JsonNumber* v)
    {
        std::string s;
        if (v->IsDecimal()) {
            snprintf(string_buffer(s, 128), 128, "%f", v->ToFloat());
        }
        else {
			if (v->Is64()) {
				if (v->IsUnsigned()) {
                    snprintf(string_buffer(s, 128), 128, "%llu", v->ToUint64());
				}
				else {
					snprintf(string_buffer(s, 128), 128, "%lld", v->ToInt64());
				}
			}
			else {
				if (v->IsUnsigned()) {
					snprintf(string_buffer(s, 128), 128, "%u", v->ToUint());
				}
				else {
					snprintf(string_buffer(s, 128), 128, "%d", v->ToInt());
				}
			}
        }
        os << s;
    }
    
    void WriteString(std::basic_ostream<char>& os, const JsonString* v)
    {
        std::string ws = escape(v->GetString());
        os << ('"');
        os << ws;
        os << ('"');
    }
    
    void WriteObject(std::basic_ostream<char>& os, const JsonObject* v)
    {
        os << ('{');
        if (IsFormatted()) {
            IncreaseIndent();
        }
        
        for (JsonObject::const_iterator it = v->cbegin(); it != v->cend(); ++it) {
            
            if (it != v->cbegin()) {
                os << ',';
            }
            if (IsFormatted()) {
                WriteLineEnd(os);
                WriteIndent(os);
            }
            
            std::string key = escape(it->first);
            os << '"';
            os << key;
            os << '"';
            os << ':';
            if (IsFormatted()) {
                os << ' ';
            }
            WriteValue(os, it->second);
            
        }
        
        if (IsFormatted()) {
            DecreaseIndent();
            WriteLineEnd(os);
            WriteIndent(os);
        }
        os << ('}');
    }
    
    void WriteArray(std::basic_ostream<char>& os, const JsonArray* v)
    {
        os << ('[');
        if (IsFormatted()) {
            IncreaseIndent();
        }
        
        for (JsonArray::const_iterator it = v->cbegin(); it != v->cend(); ++it) {
            
            if (it != v->cbegin()) {
                os << ',';
            }
            if (IsFormatted()) {
                WriteLineEnd(os);
                WriteIndent(os);
            }
            WriteValue(os, (*it));
        }
        
        if (IsFormatted()) {
            DecreaseIndent();
            WriteLineEnd(os);
            WriteIndent(os);
        }
        os << (']');
    }
    
private:
    int _indent;
    WriterType _type;
};
            

class JsonStringWriter : public Writer
{
public:
    JsonStringWriter() : Writer()
    {
    }
    virtual ~JsonStringWriter()
    {
    }
    
    std::string Write(const JsonValue* v, WriterType type = Compacted)
    {
        SetType(type);
        std::basic_ostringstream<char> os;
        WriteValue(os, v);
        return os.str();
    }
};
    
class JsonParser
{
public:
    JsonParser() : _errcode(0), _errpos(0)
    {
    }

    virtual ~JsonParser()
    {
    }

    inline size_t GetErrorPosition() const { return _errpos; }
    inline int GetErrorCode() const { return _errcode; }
    inline bool Succeeded() const { return (0 == _errcode); }

    typedef enum JsonParseError {
        JESuccess = 0,
        JEInvalidRootType,
        JEUnexpectedEnding,
        JEUnexpectedChar,
        JEInvalidNullValue,
        JEInvalidBoolValue,
        JEInvalidNumberValue,
        JEInvalidStringValue,
        JEInvalidObjectKey,
        JEInvalidObjectValue,
        JEInvalidArrayValue,
        JEInvalidEscapedChar,
        JEInvalidNumberChar,
        JEInvalidHexChar,
        JEMissingComma,
        JEMissingColon,
        JEMissingLeftQuotationMark,
        JEMissingRightQuotationMark,
        JEMissingObjectLeftBrace,
        JEMissingObjectRightBrace,
        JEMissingArrayLeftBracket,
        JEMissingArrayRightBracket,
        JEUnknown
    } JsonParseError;

    JsonValue* Parse()
    {
        _errcode = 0;
        _errpos = 0;
        char ch = PeekNextNwChar();
        if (ch == '{') {
            return ParseObject();
        }
        else if (ch == '[') {
            return ParseArray();
        }
        else {
            _errcode = JEInvalidRootType;
            return nullptr;
        }
    }

protected:
    virtual char GetNextChar() = 0;
    virtual char PeekNextChar() = 0;
    virtual bool ReachEnd() = 0;
    
    virtual char GetNextNwChar()
    {
        char ch = 0;
        while (0 != (ch = GetNextChar()) && iswspace((int)ch));
        return ch;
    }
    
    virtual char PeekNextNwChar()
    {
        char ch = 0;
        while (0 != (ch = PeekNextChar()) && iswspace((int)ch)) {
            (void)GetNextChar();
        }
        return ch;
    }
    
    void IncreasePos() { ++_errpos; }
                
protected:
    JsonValue* ParseValue()
    {
        JsonValue* pv = nullptr;
        char ch = PeekNextNwChar();
        switch (ch)
        {
            case 0:
                _errcode = JEUnexpectedEnding;
                break;
            case '{':
                pv = ParseObject();
                break;
            case '[':
                pv = ParseArray();
                break;
            case '"':
                pv = ParseString();
                break;
            case 'N':
            case 'n':
                pv = ParseNull();
                break;
            case 'T':
            case 't':
            case 'F':
            case 'f':
                pv = ParseBool();
                break;
            case '-':
            case '.':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                pv = ParseNumber();
                break;
            default:
                _errcode = JEUnexpectedChar;
                break;
        }
        
        return pv;
    }
    
    JsonValue* ParseObject()
    {
        char ch = GetNextChar();
        if ('{' != ch) {
            _errcode = ReachEnd() ? JEUnexpectedEnding : JEMissingObjectLeftBrace;
            return nullptr;
        }
        
        JsonObject* po = JsonValue::CreateObject();
        if (nullptr == po) {
            return po;
        }
        
        do {
            
            ch = PeekNextNwChar();
            
            // End of Object
            if ('}' == ch) {
                (void)GetNextChar();
                break;
            }
            
            // Empty Object item
            if (',' == ch) {
                (void)GetNextChar();
                continue;
            }
            
            // First: Key
            if ('"' != ch) {
                _errcode = ReachEnd() ? JEUnexpectedEnding : JEMissingLeftQuotationMark;
                goto _error;
            }
            
            std::string key;
            
            if (!ParseQuotaString(key)) {
                goto _error;
            }
            
            if (key.empty()) {
                _errcode = JEInvalidObjectKey;
                goto _error;
            }
            
            ch = GetNextNwChar();
            if (':' != ch) {
                _errcode = ReachEnd() ? JEUnexpectedEnding : JEMissingColon;
                goto _error;
            }
            
            JsonValue* value = ParseValue();
            if (nullptr == value)
                goto _error;
            
            if (!po->set(key, value)) {
                goto _error;
            }
            
        } while (true);
        
        goto _succeed;
    _error:
        if (po) {
            delete po;
            po = nullptr;
        }
        
    _succeed:
        return po;
    }
                
    JsonValue* ParseArray()
    {
        char ch = GetNextChar();
        if ('[' != ch) {
            _errcode = ReachEnd() ? JEUnexpectedEnding : JEMissingArrayLeftBracket;
            return nullptr;
        }
                    
        JsonArray* pa = JsonValue::CreateArray();
        if (nullptr == pa) {
//            SetLastError(RMCCORE_ERROR_NOT_ENOUGH_MEMORY);
            return pa;
        }
                    
        do {
            
            ch = PeekNextNwChar();
                        
            // End of Array
            if (']' == ch) {
                (void)GetNextChar();
                break;
            }
                        
            // Empty Array item
            if (',' == ch) {
                (void)GetNextChar();
                continue;
            }
                        
            JsonValue* value = ParseValue();
            if (nullptr == value)
                goto _error;
                        
            if (!pa->push_back(value)) {
                goto _error;
            }
                        
        } while (true);
                    
        goto _succeed;
_error:
        if (pa) {
            delete pa;
            pa = nullptr;
        }
                    
_succeed:
        return pa;
    }
                
    JsonValue* ParseNull()
    {
        if (0 != icompare(GetNextChar(), 'N')) {
            _errcode = ReachEnd() ? JEUnexpectedEnding : JEInvalidNullValue;
            return nullptr;
        }
        if (0 != icompare(GetNextChar(), 'u')) {
            _errcode = ReachEnd() ? JEUnexpectedEnding : JEInvalidNullValue;
            return nullptr;
        }
        if (0 != icompare(GetNextChar(), 'l')) {
            _errcode = ReachEnd() ? JEUnexpectedEnding : JEInvalidNullValue;
            return nullptr;
        }
        if (0 != icompare(GetNextChar(), 'l')) {
            _errcode = ReachEnd() ? JEUnexpectedEnding : JEInvalidNullValue;
            return nullptr;
        }
        
        return JsonValue::CreateNull();
    }
                
    JsonValue* ParseBool()
    {
        bool v = false;
        char ch = GetNextChar();
        if (0 == icompare(ch, 'T')) {
            if (0 != icompare(GetNextChar(), 'r')) {
                _errcode = ReachEnd() ? JEUnexpectedEnding : JEInvalidBoolValue;
                return nullptr;
            }
            if (0 != icompare(GetNextChar(), 'u')) {
                _errcode = ReachEnd() ? JEUnexpectedEnding : JEInvalidBoolValue;
                return nullptr;
            }
            if (0 != icompare(GetNextChar(), 'e')) {
                _errcode = ReachEnd() ? JEUnexpectedEnding : JEInvalidBoolValue;
                return nullptr;
            }
            v = true;
        }
        else if (0 == icompare(ch, 'F')) {
            if (0 != icompare(GetNextChar(), 'a')) {
                _errcode = ReachEnd() ? JEUnexpectedEnding : JEInvalidBoolValue;
                return nullptr;
            }
            if (0 != icompare(GetNextChar(), 'l')) {
                _errcode = ReachEnd() ? JEUnexpectedEnding : JEInvalidBoolValue;
                return nullptr;
            }
            if (0 != icompare(GetNextChar(), 's')) {
                _errcode = ReachEnd() ? JEUnexpectedEnding : JEInvalidBoolValue;
                return nullptr;
            }
            if (0 != icompare(GetNextChar(), 'e')) {
                _errcode = ReachEnd() ? JEUnexpectedEnding : JEInvalidBoolValue;
                return nullptr;
            }
            v = false;
        }
        else {
            _errcode = JEInvalidBoolValue;
            return nullptr;
        }
        
        return JsonValue::CreateBool(v);
    }
                
    JsonValue* ParseNumber()
    {
        std::string s;
        bool    is_float = false;
        bool    is_exponent = false;
        bool    is_signed = false;
        
        char ch = GetNextChar();
        if ('-' == ch) {
            is_signed = true;
            ch = GetNextChar();
        }
        
       if ('.' == ch){
            s.push_back('0');
            is_float = true;
            ch = GetNextChar();
        }
        
        if (!isdigit(ch)) {
            _errcode = ReachEnd() ? JEUnexpectedEnding: JEInvalidNumberChar;
            return nullptr;
        }
        s.push_back(ch);
        
        do {
            
            ch = PeekNextChar();
            if (!isdigit(ch) && '.' != ch && 'E' != ch && 'e' != ch) {
                break;
            }
            
            (void)GetNextChar();
            if (ch == '.') {
                if (is_float) {
                    _errcode = JEInvalidNumberChar;
                    return nullptr;
                }
                is_float = true;
                s.push_back(ch);
            }
            else if (ch == 'E' || ch == 'e') {
                if (is_exponent) {
                    _errcode = JEInvalidNumberChar;
                    return nullptr;
                }
                is_exponent = true;
                s.push_back(ch);
            }
            else {
                s.push_back(ch);
            }
            
        } while (true);
        
        JsonValue* p = nullptr;
        try {
            if (is_float || is_exponent) {
                double f = std::stod(s);
                p = JsonValue::CreateNumber(f);
            }
            else {
                long long n = std::stoll(s);
                p = JsonValue::CreateNumber(n);
            }
        }
        catch (std::exception& e) {
            UNREFERENCED_PARAMETER(e);
            _errcode = JEInvalidNumberChar;
        }
        
        return p;
    }
    
    JsonValue* ParseString()
    {
        std::string s;
        
        if (!ParseQuotaString(s)) {
            return nullptr;
        }
        
        return JsonValue::CreateString(s);
    }
    
    bool ParseQuotaString(std::string& s)
    {
        bool result = false;
        
        char ch = GetNextChar();
        if (ch != '\"') {
            _errcode = ReachEnd() ? JEUnexpectedEnding : JEMissingLeftQuotationMark;
            return false;
        }
        
        do {
            
            ch = GetNextChar();
            if (ReachEnd()) {
                _errcode = JEUnexpectedEnding;
                return false;
            }
            
            // Finished
            if (ch == '\"') {
                result = true;
                break;
            }
            
            if (ch != '\\') {
                s.push_back(ch);
            }
            else {
                // Skip '\\'
                ch = GetNextChar();
                
                switch (ch)
                {
                    case 0:
                        _errcode = JEUnexpectedEnding;
                        break;
                    case '\"':
                        s.push_back('\"');
                        break;
                    case '\\':
                        s.push_back('\\');
                        break;
                    case '/':
                        s.push_back('/');
                        break;
                    case 'b':
                        s.push_back('\b');
                        break;
                    case 'f':
                        s.push_back('\f');
                        break;
                    case 'r':
                        s.push_back('\r');
                        break;
                    case 'n':
                        s.push_back('\n');
                        break;
                    case 't':
                        s.push_back('\t');
                        break;
                    case 'u':
                        // A four-hexdigit Unicode character.
                        // Transform into a 16 bit code point.
                    {
                        char wc = 0;
                        for (int i = 0; i < 4; i++) {
                            int n = (unsigned char)ctoi(GetNextChar());
                            if (n < 0) {
                                _errcode = JEInvalidHexChar;
                                return false;
                            }
                            wc <<= 4;
                            wc |= (((unsigned char)n) & 0xF);
                        }
                        s.push_back(wc);
                    }
                        break;
                    default:
                        _errcode = JEInvalidEscapedChar;
                        break;
                }
                
                if (JESuccess != _errcode)
                    break;
            }
            
        } while (true);
        
        return result;
    }
    
private:
    size_t _errpos;
    int    _errcode;
};
    
class JsonStringParser : public JsonParser
{
public:
    JsonStringParser() : JsonParser(){}
    virtual ~JsonStringParser() {}
    
    JsonValue* Parse(const std::string& s)
    {
        _s = s.c_str();
        return JsonParser::Parse();
    }
    
protected:
    virtual char GetNextChar()
    {
        if (!ReachEnd()) {
            IncreasePos();
            return (*_s++);
        }
        return 0;
    }
    
    virtual char PeekNextChar()
    {
        return ReachEnd() ? 0 : (*_s);
    }
    
    virtual bool ReachEnd()
    {
        if (nullptr == _s) return true;
        return (0 == *_s) ? true : false;
    }
    
private:
    const char* _s;
};

class JsonDocument
{
public:
    JsonDocument() : _root(nullptr)
    {
    }
    
    virtual ~JsonDocument()
    {
        Clear();
    }
    
    bool Create()
    {
        _root = (JsonValue*)JsonValue::CreateObject();
        return !Empty();
    }
    
    bool Empty() const
    {
        return (nullptr == _root);
    }
    
    void Clear()
    {
        if (nullptr != _root) {
            delete _root;
            _root = nullptr;
        }
    }
    
    inline JsonValue* GetRoot() { return _root; }
    inline const JsonValue* GetRoot() const { return _root; }
    
    bool LoadJsonString(const std::string& s, int* err_code = nullptr, size_t* err_pos = nullptr)
    {
        JsonStringParser parser;
        _root = parser.Parse(s);
        if (err_code) *err_code = parser.GetErrorCode();
        if (err_pos) *err_pos = (0 != parser.GetErrorCode()) ? parser.GetErrorPosition() : 0;
        return !Empty();
    }
    
protected:
    JsonValue* _Move()
    {
        JsonValue* tmp = _root;
        _root = nullptr;
        return tmp;
    }
    
private:
    // No copy is allowed
    JsonDocument(const JsonDocument& rhs) {}
    
private:
    JsonValue*  _root;
};
    
}

#endif /* __RMCCORE_UTILS_JSON_H__ */
