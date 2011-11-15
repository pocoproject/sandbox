///
/// \package pg
/// \file PqSQLInternal.h
///
/// \author Marian Krivos <marian.krivos@rsys.sk>
/// \date Sep 17, 2011 - 12:00:16 PM
/// \brief deklaracia typu
///
/// (C) Copyright 2011 R-SYS,s.r.o
/// All rights reserved.
///

#ifndef pocoDataPQSQLINTERNAL_H_
#define pocoDataPQSQLINTERNAL_H_

#include <vector>
#include <cstring>

#define PgSQL_TYPE_NULL       0 // FIXME: NULL

#define PgSQL_TYPE_BOOL      16 // QBOOLOID
#define PgSQL_TYPE_INT8      18 // OINT1OID
#define PgSQL_TYPE_INT16     21 // QINT2OID
#define PgSQL_TYPE_INT32     23 // QINT4OID
#define PgSQL_TYPE_INT64     20 // QINT8OID

#define PgSQL_TYPE_BYTEARRAY 17 // QBYTEARRAY (BINARY)
#define PgSQL_TYPE_STRING    25 // STRING (NOT BIN)

#define PgSQL_TYPE_FLOAT     700 // QFLOAT4OID
#define PgSQL_TYPE_DOUBLE    701 // QFLOAT8OID

#define PgSQL_TYPE_DATE      1082 // QDATEOID
#define PgSQL_TYPE_TIME      1083 // QTIMEOID
#define PgSQL_TYPE_TIMESTAMP 1114 // QTIMESTAMPOID

/*
#define QABSTIMEOID 702
#define QRELTIMEOID 703
#define QTIMETZOID 1266
#define QTIMESTAMPTZOID 1184
#define QOIDOID 2278
#define PgSQL_TYPE_NUMERIC 1700 // QNUMERICOID
#define QREGPROCOID 24
#define QXIDOID 28
#define QCIDOID 29
*/
struct PgSQL_option
{
};

class PgSQL_BIND
{
public:
    PgSQL_BIND() :
        _oid(0),
        _paramValue(0),
        _paramLenght(0)
    {
    }

    PgSQL_BIND(int oid, const char* paramValue, int paramLenght) :
        _oid(oid),
        _paramValue(paramLenght),
        _paramLenght(paramLenght)
    {
        memcpy(&_paramValue[0], paramValue, paramLenght);
    }

    PgSQL_BIND(const PgSQL_BIND& old) :
        _oid(old._oid),
        _paramValue(old._paramValue),
        _paramLenght(old._paramLenght)
    {
    }

    PgSQL_BIND& operator =(const PgSQL_BIND& old)
    {
        _oid = old._oid;
        _paramValue = old._paramValue;
        _paramLenght = old._paramLenght;
        return *this;
    }

    int _oid;
    std::vector<char> _paramValue;
    int _paramLenght;
};

#endif /* pocoDataPQSQLINTERNAL_H_ */
