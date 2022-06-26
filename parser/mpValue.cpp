/*
<pre>
			 __________                                 ____  ___
  _____  __ _\______   \_____ _______  ______ __________\   \/  /
 /     \|  |  \     ___/\__  \\_  __ \/  ___// __ \_  __ \     /
|  Y Y  \  |  /    |     / __ \|  | \/\___ \\  ___/|  | \/     \
|__|_|  /____/|____|    (____  /__|  /____  >\___  >__| /___/\  \
	  \/                     \/           \/     \/           \_/
Copyright (C) 2016, Ingo Berg
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
</pre>
*/
#include "mpValue.h"
#include "mpError.h"
#include "mpValueCache.h"


MUP_NAMESPACE_START

//------------------------------------------------------------------------------
/** \brief Construct an empty value object of a given type.
	\param cType The type of the value to construct (default='v').
	*/
	Value::Value(char_type cType)
	:IValue(cmVAL)
	//, m_val(0, 0)
	// , data.m_psVal(nullptr)
	// , data.m_pvVal(nullptr)
	, m_cType(cType)
	, m_iFlags(flNONE)
	, m_pCache(nullptr)
{
	data.m_fval = 0;
	// strings and arrays must allocate their memory
	switch (cType)
	{
	case 's': data.m_psVal = new string_type(); break;
	case 'm': data.m_pvVal = new matrix_type(0, Value(0.0)); break;
	}
}

//---------------------------------------------------------------------------
Value::Value(int_type a_iVal)
	:IValue(cmVAL)
	// , m_val((float_type)a_iVal, 0)
	// , data.m_psVal(nullptr)
	// , data.m_pvVal(nullptr)
	, m_cType('i')
	, m_iFlags(flNONE)
	, m_pCache(nullptr)
{data.m_ival = a_iVal;}

//---------------------------------------------------------------------------
Value::Value(bool_type a_bVal)
	:IValue(cmVAL)
	// , m_val((float_type)a_bVal, 0)
	// , data.m_psVal(nullptr)
	// , data.m_pvVal(nullptr)
	, m_cType('b')
	, m_iFlags(flNONE)
	, m_pCache(nullptr)
{data.m_bval = a_bVal; }

//---------------------------------------------------------------------------
Value::Value(string_type a_sVal)
	:IValue(cmVAL)
	// , m_val()
	// , data.m_pvVal(nullptr)
	, m_cType('s')
	, m_iFlags(flNONE)
	, m_pCache(nullptr)
{data.m_psVal = new string_type(a_sVal);}

//---------------------------------------------------------------------------
Value::Value(int_type array_size, float_type v)
	:IValue(cmVAL)
	// , m_val()
	// , data.m_psVal(nullptr)
	, m_cType('m')
	, m_iFlags(flNONE)
	, m_pCache(nullptr)
{data.m_pvVal = new matrix_type(array_size, Value(v));}

//---------------------------------------------------------------------------
/** \brief Create a m x n matrix
*/
Value::Value(int_type m, int_type n, float_type v)
	:IValue(cmVAL)
	// , m_val()
	// , data.m_psVal(nullptr)
	, m_cType('m')
	, m_iFlags(flNONE)
	, m_pCache(nullptr)
{data.m_pvVal = new matrix_type(m, n, Value(v));}

//---------------------------------------------------------------------------
Value::Value(const char_type* a_szVal)
	:IValue(cmVAL)
	// , m_val()
	// , data.m_pvVal(nullptr)
	, m_cType('s')
	, m_iFlags(flNONE)
	, m_pCache(nullptr)
{data.m_psVal = new string_type(a_szVal);}

//---------------------------------------------------------------------------
Value::Value(const cmplx_type& v)
	:IValue(cmVAL)
	, m_val(v)
	// , data.m_psVal(nullptr)
	// , data.m_pvVal(nullptr)
	, m_cType('c')
	, m_iFlags(flNONE)
	, m_pCache(nullptr)
{
	// modified as suggested here: https://github.com/beltoforion/muparserx/issues/98
	// m_cType = (m_val.imag() == 0) ? ((std::floor(m_val.real()) == m_val.real()) ? 'i' : 'f') : 'c';
}

//---------------------------------------------------------------------------
Value::Value(float_type val)
	:IValue(cmVAL)
	// , m_val(val, 0)
	// , data.m_psVal(nullptr)
	// , data.m_pvVal(nullptr)
	//, m_cType((val == (int_type)val) ? 'i' : 'f')
	, m_cType('f')
	, m_iFlags(flNONE)
	, m_pCache(nullptr)
{data.m_fval = val;}

//---------------------------------------------------------------------------
Value::Value(const matrix_type& val)
	:IValue(cmVAL)
	// , m_val()
	// , data.m_psVal(nullptr)
	, m_cType('m')
	, m_iFlags(flNONE)
	, m_pCache(nullptr)
{data.m_pvVal = new matrix_type(val);}

//---------------------------------------------------------------------------
Value::Value(const Value& a_Val)
	:IValue(cmVAL)
	, m_cType('f')
	// , data.m_psVal(nullptr)
	// , data.m_pvVal(nullptr)
	, m_pCache(nullptr)
{
	data.m_fval = 0;
	Assign(a_Val);
}

//---------------------------------------------------------------------------
Value::Value(const IValue& a_Val)
	:IValue(cmVAL)
	, m_cType('f')
	, m_iFlags(flNONE)
	//, data.m_psVal(nullptr)
	//, data.m_pvVal(nullptr)
	, m_pCache(nullptr)
{
	data.m_fval = 0;
	Reset();

	switch (a_Val.GetType())
	{
	case 'i':
		if (m_cType == 'm')
		{
			delete data.m_pvVal;
			data.m_pvVal = nullptr;
		}
		if (m_cType == 's')
		{
			delete data.m_psVal;
			data.m_psVal = nullptr;
		}
		data.m_ival = a_Val.GetInteger();
		break;

	case 'f':
	  if (m_cType == 'm')
		{
			delete data.m_pvVal;
			data.m_pvVal = nullptr;
		}
		if (m_cType == 's')
		{
			delete data.m_psVal;
			data.m_psVal = nullptr;
		}
		data.m_fval = a_Val.GetFloat();
		break;

	case 'b': 
		if (m_cType == 'm')
		{
			delete data.m_pvVal;
			data.m_pvVal = nullptr;
		}
		if (m_cType == 's')
		{
			delete data.m_psVal;
			data.m_psVal = nullptr;
		}
		data.m_bval = a_Val.GetBool();
		break;


	case 'c': 
		if (m_cType == 'm')
		{
			delete data.m_pvVal;
			data.m_pvVal = nullptr;
		}
		if (m_cType == 's')
		{
			delete data.m_psVal;
			data.m_psVal = nullptr;
		}
		m_val = cmplx_type(a_Val.GetReal(), a_Val.GetImag());
		break;

	case 's':
		if (m_cType == 'm')
		{
			delete data.m_pvVal;
			data.m_pvVal = nullptr;
		}
		if (m_cType == 's')
			*data.m_psVal = a_Val.GetString();
		else
			data.m_psVal = new string_type(a_Val.GetString());
		break;

	case 'm': 
		if (m_cType == 's')
		{
			delete data.m_psVal;
			data.m_psVal = nullptr;
		}
		if (m_cType == 'm')
			*data.m_pvVal = a_Val.GetArray();
		else
			data.m_pvVal = new matrix_type(a_Val.GetArray());
		break;

	case 'v': 
		if (m_cType == 'm')
		{
			delete data.m_pvVal;
			data.m_pvVal = nullptr;
		}
		if (m_cType == 's')
		{
			delete data.m_psVal;
			data.m_psVal = nullptr;
		}
		break;
	default:  MUP_FAIL(INVALID_TYPE_CODE);
	}

	m_cType = a_Val.GetType();
}

//---------------------------------------------------------------------------
Value& Value::operator=(const Value& a_Val)
{
	Assign(a_Val);
	return *this;
}

//---------------------------------------------------------------------------
/** \brief Return the matrix element at row col.

  Row and col are the indices of the matrix. If this element does not
  represent a matrix row and col must be 0 otherwise an index out of bound error
  is thrown.
  */
IValue& Value::At(const IValue& row, const IValue& col)
{
	int_type nRow = 0;
	int_type nCol = 0;
	if (!row.IsInteger())
	{
		if (row.GetType() == 'f' && row.GetFloat() == (int_type)row.GetFloat()) {
			nRow = (int_type)row.GetFloat();
		}
		else
		{
		ErrorContext errc(ecTYPE_CONFLICT_IDX, GetExprPos());
		errc.Type1 = row.GetType();
		errc.Type2 = 'i';
		throw ParserError(errc);
		}
	}
	else
	{
		nRow = row.GetInteger();
	}
	if (!col.IsInteger())
	{
		if (col.GetType() == 'f' && col.GetFloat() == (int_type)col.GetFloat()) {
			nCol = (int_type)row.GetFloat();
		}
		else
		{
			ErrorContext errc(ecTYPE_CONFLICT_IDX, GetExprPos());
			errc.Type1 = col.GetType();
			errc.Type2 = 'i';
			throw ParserError(errc);
		}
	}
	else 
	{
		nCol = col.GetInteger();
	}
		
	return At(nRow, nCol);
}

//---------------------------------------------------------------------------
IValue& Value::At(int nRow, int nCol)
{
	if (IsMatrix())
	{
		if (nRow >= data.m_pvVal->GetRows() || nCol >= data.m_pvVal->GetCols() || nRow < 0 || nCol < 0)
			throw ParserError(ErrorContext(ecINDEX_OUT_OF_BOUNDS, -1, GetIdent()));

		return data.m_pvVal->At(nRow, nCol);
	}
	else if (nRow == 0 && nCol == 0)
	{
		return *this;
	}
	else
		throw ParserError(ErrorContext(ecINDEX_OUT_OF_BOUNDS));
}

//---------------------------------------------------------------------------
Value::~Value()
{
	if (m_cType == 's')
	{
		delete data.m_psVal;
		data.m_psVal = nullptr;
	}
	else if (m_cType == 'm')
	{
		delete data.m_pvVal;
		data.m_pvVal = nullptr;
	}
}

//---------------------------------------------------------------------------
IToken* Value::Clone() const
{
	return new Value(*this);
}

//---------------------------------------------------------------------------
Value* Value::AsValue()
{
	return this;
}

//---------------------------------------------------------------------------
/** \brief Copy constructor. */
void Value::Assign(const Value& ref)
{
	if (this == &ref)
		return;

	switch (ref.GetType())
	{
	case 'i':
		if (m_cType == 's') {
			delete data.m_psVal;
			data.m_psVal = nullptr;
		}
		else if (m_cType == 'm') {
			delete data.m_pvVal;
			data.m_pvVal = nullptr;
		}
		data.m_ival = ref.GetInteger();
		break;

	case 'f':
		if (m_cType == 's') {
			delete data.m_psVal;
			data.m_psVal = nullptr;
		}
		else if (m_cType == 'm') {
			delete data.m_pvVal;
			data.m_pvVal = nullptr;
		}
		data.m_fval = ref.GetFloat();
		break;

	case 'b': 
		if (m_cType == 's') {
			delete data.m_psVal;
			data.m_psVal = nullptr;
		}
		else if (m_cType == 'm') {
			delete data.m_pvVal;
			data.m_pvVal = nullptr;
		}
		data.m_bval = ref.GetBool();
		break;


	case 'c': 
		if (m_cType == 's') {
			delete data.m_psVal;
			data.m_psVal = nullptr;
		}
		else if (m_cType == 'm') {
			delete data.m_pvVal;
			data.m_pvVal = nullptr;
		}
		m_val = cmplx_type(ref.GetReal(), ref.GetImag());
		break;

	case 's':
		if (m_cType == 'm') {
			delete data.m_pvVal;
			data.m_pvVal = nullptr;
		}
		if (m_cType == 's')
			*data.m_psVal = ref.GetString();
		else
			data.m_psVal = new string_type(ref.GetString());
		break;

	case 'm': 
		if (m_cType == 's') {
			delete data.m_psVal;
			data.m_psVal = nullptr;
		}
		if (m_cType == 'm')
			*data.m_pvVal = ref.GetArray();
		else
			data.m_pvVal = new matrix_type(ref.GetArray());
		break;

	case 'v':
		if (m_cType == 's') {
			delete data.m_psVal;
			data.m_psVal = nullptr;
		}
		else if (m_cType == 'm') {
			delete data.m_pvVal;
			data.m_pvVal = nullptr;
		}
	break;
	default:  MUP_FAIL(INVALID_TYPE_CODE);
	}

	// m_val = ref.m_val;
	m_cType = ref.m_cType;
	m_iFlags = ref.m_iFlags;

	// Do NOT access ref beyound this point! If you do, "unboxing" of
	// a 1 x 1 matrix using:
	//
	// this->Assign(m_pvVal->At(0,0));
	// 
	// will blow up in your face since ref will become invalid at them very
	// moment you delete m_pvVal!
}

//---------------------------------------------------------------------------
void Value::Reset()
{
	// m_val = cmplx_type(0, 0);

	if (m_cType == 's') {
		delete data.m_psVal;
		data.m_psVal = nullptr;
	}
	else if (m_cType == 'm') {
		delete data.m_pvVal;
		data.m_pvVal = nullptr;
	}

	data.m_fval = 0;
	m_cType = 'f';
	m_iFlags = flNONE;
}

//---------------------------------------------------------------------------
IValue& Value::operator=(bool val)
{
	// m_val = cmplx_type((float_type)val, 0);
	if (m_cType == 's') {
		delete data.m_psVal;
		data.m_psVal = nullptr;
	}
	else if (m_cType == 'm') {
		delete data.m_pvVal;
		data.m_pvVal = nullptr;
	}

	data.m_bval = val;
	m_cType = 'b';
	m_iFlags = flNONE;
	return *this;
}

//---------------------------------------------------------------------------
IValue& Value::operator=(int_type a_iVal)
{
	// m_val = cmplx_type(a_iVal, 0);
	if (m_cType == 's') {
		delete data.m_psVal;
		data.m_psVal = nullptr;
	}
	else if (m_cType == 'm') {
		delete data.m_pvVal;
		data.m_pvVal = nullptr;
	}

	data.m_ival = a_iVal;
	m_cType = 'i';
	m_iFlags = flNONE;
	return *this;
}

//---------------------------------------------------------------------------
IValue& Value::operator=(float_type val)
{
	// m_val = cmplx_type(val, 0);
	if (m_cType == 's') {
		delete data.m_psVal;
		data.m_psVal = nullptr;
	}
	else if (m_cType == 'm') {
		delete data.m_pvVal;
		data.m_pvVal = nullptr;
	}

	data.m_fval = val;
	// m_cType = (val == (int_type)val) ? 'i' : 'f';
	m_cType = 'f';
	m_iFlags = flNONE;
	return *this;
}

//---------------------------------------------------------------------------
IValue& Value::operator=(string_type a_sVal)
{
	if (m_cType == 'm') {
		delete data.m_pvVal;
		data.m_pvVal = nullptr;
	}

	if (m_cType == 's')
		*data.m_psVal = a_sVal;
	else
		data.m_psVal = new string_type(a_sVal);

	// delete data.m_pvVal;
	// data.m_pvVal = nullptr;

	m_cType = 's';
	m_iFlags = flNONE;
	return *this;
}

//---------------------------------------------------------------------------
IValue& Value::operator=(const char_type* a_szVal)
{
	// m_val = cmplx_type();
	if (m_cType == 'm') {
		delete data.m_pvVal;
		data.m_pvVal = nullptr;
	}

	if (m_cType == 's')
		*data.m_psVal = a_szVal;
	else
		data.m_psVal = new string_type(a_szVal);

	// delete data.m_pvVal;
	// data.m_pvVal = nullptr;

	m_cType = 's';
	m_iFlags = flNONE;
	return *this;
}

//---------------------------------------------------------------------------
IValue& Value::operator=(const matrix_type& a_vVal)
{
	// m_val = cmplx_type(0, 0);
	if (m_cType == 's') {
		delete data.m_psVal;
		data.m_psVal = nullptr;
	}
	// delete data.m_psVal;
	// data.m_psVal = nullptr;

	if (m_cType == 'm')
		*data.m_pvVal = a_vVal;
	else
		data.m_pvVal = new matrix_type(a_vVal);

	m_cType = 'm';
	m_iFlags = flNONE;

	return *this;
}

//---------------------------------------------------------------------------
IValue& Value::operator=(const cmplx_type& val)
{
	if (m_cType == 's') {
		delete data.m_psVal;
		data.m_psVal = nullptr;
	}
	else if (m_cType == 'm') {
		delete data.m_pvVal;
		data.m_pvVal = nullptr;
	}

	m_val = val;
	// modified as suggested here: https://github.com/beltoforion/muparserx/issues/98
	// m_cType = (m_val.imag() == 0) ? ((std::floor(m_val.real()) == m_val.real()) ? 'i' : 'f') : 'c';
	m_cType = 'c';
	m_iFlags = flNONE;

	return *this;
}

//---------------------------------------------------------------------------
IValue& Value::operator+=(const IValue& val)
{
	if (GetType() == 'i') {
		if (val.GetType() == 'i') {
			data.m_ival += val.GetInteger();
		}
		else if (val.GetType() == 'f') {
			data.m_fval += val.GetFloat();
			m_cType = 'f';
		}
		else if (val.GetType() == 'c') {
			cmplx_type leftOprand = cmplx_type(data.m_ival, 0);
			m_val = leftOprand + val.GetComplex();
			m_cType = 'c';
		}
		else {
			// Type conflict
			throw ParserError(ErrorContext(ecTYPE_CONFLICT_FUN, -1, _T("+"), GetType(), val.GetType(), 2));
		}
	}
	else if (GetType() == 'f') {
		if (val.GetType() == 'f') {
			data.m_fval += val.GetFloat();
		}
		else if (val.GetType() == 'i') {
			data.m_fval += val.GetInteger();
		}
		else if (val.GetType() == 'c') {
			cmplx_type leftOprand = cmplx_type(data.m_fval, 0);
			m_val = leftOprand + val.GetComplex();
			m_cType = 'c';
		}
		else {
			// Type conflict
			throw ParserError(ErrorContext(ecTYPE_CONFLICT_FUN, -1, _T("+"), GetType(), val.GetType(), 2));
		}
	}
	else if (IsComplex() && val.IsComplex())
	{
		// Scalar/Scalar addition
		m_val += val.GetComplex();

		// modified as suggested here: https://github.com/beltoforion/muparserx/issues/98
		// m_cType = (m_val.imag() == 0) ? ((std::floor(m_val.real()) == m_val.real()) ? 'i' : 'f') : 'c';
		m_cType = 'c';
	}
	else if (IsMatrix() && val.IsMatrix())
	{
		// Matrix/Matrix addition
		assert(data.m_pvVal);
		*data.m_pvVal += val.GetArray();
	}
	else if (IsString() && val.IsString())
	{
		// string/string addition
		assert(data.m_psVal);
		*data.m_psVal += val.GetString();
	}
	else
	{
		// Type conflict
		throw ParserError(ErrorContext(ecTYPE_CONFLICT_FUN, -1, _T("+"), GetType(), val.GetType(), 2));
	}

	return *this;
}

//---------------------------------------------------------------------------
IValue& Value::operator-=(const IValue& val)
{
	if (GetType() == 'i') {
		if (val.GetType() == 'i') {
			data.m_ival -= val.GetInteger();
		}
		else if (val.GetType() == 'f') {
			data.m_fval -= val.GetFloat();
			m_cType = 'f';
		}
		else if (val.GetType() == 'c') {
			cmplx_type leftOprand = cmplx_type(data.m_ival, 0);
			m_val = leftOprand - val.GetComplex();
			m_cType = 'c';
		}
		else {
			// Type conflict
			throw ParserError(ErrorContext(ecTYPE_CONFLICT_FUN, -1, _T("+"), GetType(), val.GetType(), 2));
		}
	}
	else if (GetType() == 'f') {
		if (val.GetType() == 'f') {
			data.m_fval -= val.GetFloat();
		}
		else if (val.GetType() == 'i') {
			data.m_fval -= val.GetInteger();
		}
		else if (val.GetType() == 'c') {
			cmplx_type leftOprand = cmplx_type(data.m_fval, 0);
			m_val = leftOprand - val.GetComplex();
			m_cType = 'c';
		}
		else {
			// Type conflict
			throw ParserError(ErrorContext(ecTYPE_CONFLICT_FUN, -1, _T("+"), GetType(), val.GetType(), 2));
		}
	}
	else if (IsComplex() && val.IsComplex())
	{
		// Scalar/Scalar addition
		m_val -= val.GetComplex();

		// modified as suggested here: https://github.com/beltoforion/muparserx/issues/98
		// m_cType = (m_val.imag() == 0) ? ((std::floor(m_val.real()) == m_val.real()) ? 'i' : 'f') : 'c';
		m_cType = 'c';
	}
	else if (IsMatrix() && val.IsMatrix())
	{
		// Matrix/Matrix addition
		assert(data.m_pvVal);
		*data.m_pvVal -= val.GetArray();
	}
	else
	{
		// There is a typeconflict:
		throw ParserError(ErrorContext(ecTYPE_CONFLICT_FUN, -1, _T("-"), GetType(), val.GetType(), 2));
	}

	return *this;
}

//---------------------------------------------------------------------------
/** \brief Assign a value with multiplication
	\param val The value to multiply to this

	When multiplying to values with each value representing a matrix type
	the result is checked whether it is a 1 x 1 matrix. If so the value is
	"unboxed" and stored directly in this value object. It is no longer
	treated as a matrix internally.
	*/
IValue& Value::operator*=(const IValue& val)
{
	if (GetType() == 'i') {
		if (val.GetType() == 'i') {
			data.m_ival *= val.GetInteger();
		}
		else if (val.GetType() == 'f') {
			int_type leftOprand = data.m_ival;
			data.m_fval = leftOprand * val.GetFloat();
			m_cType = 'f';
		}
		else if (val.GetType() == 'c') {
			cmplx_type leftOprand = cmplx_type(data.m_ival, 0);
			m_val = leftOprand * val.GetComplex();
			m_cType = 'c';
		}
		else if (val.GetType() == 'm') {
			Value prod = val * (*this);
			Assign(prod);
		}
		else {
			// Type conflict
			throw ParserError(ErrorContext(ecTYPE_CONFLICT_FUN, -1, _T("+"), GetType(), val.GetType(), 2));
		}
	}
	else if (GetType() == 'f') {
		if (val.GetType() == 'f') {
			data.m_fval *= val.GetFloat();
		}
		else if (val.GetType() == 'i') {
			data.m_fval *= val.GetInteger();
		}
		else if (val.GetType() == 'c') {
			cmplx_type leftOprand = cmplx_type(data.m_fval, 0);
			m_val = leftOprand * val.GetComplex();
			m_cType = 'c';
		}
		else if (val.GetType() == 'm') {
			Value prod = val * (*this);
			Assign(prod);
		}
		else {
			// Type conflict
			throw ParserError(ErrorContext(ecTYPE_CONFLICT_FUN, -1, _T("+"), GetType(), val.GetType(), 2));
		}
	}
	else if (IsComplex() && val.IsComplex())
	{
		// Scalar/Scalar multiplication
		m_val *= val.GetComplex();

		// modified as suggested here: https://github.com/beltoforion/muparserx/issues/98
		// m_cType = (m_val.imag() == 0) ? ((std::floor(m_val.real()) == m_val.real()) ? 'i' : 'f') : 'c';
		m_cType = 'c';
		if (m_val.imag() == 0) {
			if (std::floor(m_val.real()) == m_val.real())
			{
				data.m_ival = m_val.real();
				m_cType = 'i';
			}
			else
			{
				data.m_fval = m_val.real();
				m_cType = 'f';
			}
		}
	}
	else if (IsMatrix() && val.IsMatrix())
	{
		// Matrix/Matrix addition
		assert(data.m_pvVal);
		*data.m_pvVal *= val.GetArray();

		// The result may actually be a scalar value, i.e. the scalar product of
		// two vectors.
		if (data.m_pvVal->GetCols() == 1 && data.m_pvVal->GetRows() == 1)
		{
			Value prod = data.m_pvVal->At(0, 0);
			Assign(prod);
		}
	}
	else if (IsMatrix() && val.IsScalar())
	{
		*data.m_pvVal *= val;
	}
	else if (IsScalar() && val.IsMatrix())
	{
		// transform this into a matrix and multiply with rhs
		Value prod = val * (*this);
		Assign(prod);
	}
	else
	{
		// Type conflict
		ErrorContext errc(ecTYPE_CONFLICT_FUN, -1, _T("*"));
		errc.Type1 = GetType();
		errc.Type2 = 'm'; //val.GetType();
		errc.Arg = 2;
		throw ParserError(errc);
	}

	return *this;
}

//---------------------------------------------------------------------------
/** \brief Returns a character representing the type of this value instance.
	\return m_cType Either one of 'c' for comlex, 'i' for integer,
	'f' for floating point, 'b' for boolean, 's' for string or
	'm' for matrix values.
	*/
char_type Value::GetType() const
{
	return m_cType;
}

//---------------------------------------------------------------------------
/** \brief Return the value as an integer.

  This function should only be called if you really need an integer value and
  want to make sure your either get one or throw an exception if the value
  can not be implicitely converted into an integer.
  */
int_type Value::GetInteger() const
{
	// float_type v = m_val.real();
	CheckType('i');

	// return (int_type)v;
	return data.m_ival;
}

//---------------------------------------------------------------------------
float_type Value::GetFloat() const
{
	// because as in union , data.m_fval and data.m_ival share the same memory 
	if (m_cType == 'f') 
	{
		return data.m_fval;
	}
	//    errc.Type1 = (!row.IsI
	else if (m_cType == 'i')
	{
		return data.m_ival;
	}
	else
	{
		CheckType('f');
	}
	// return m_val.real();
}

//---------------------------------------------------------------------------
/** \brief Get the imaginary part of the value.
	\throw ParserError in case this value represents a string or a matrix
	*/
float_type Value::GetImag() const
{
	CheckType('c');
	return m_val.imag();
}


float_type Value::GetReal() const
{
	CheckType('c');
	return m_val.real();
}

//---------------------------------------------------------------------------
/** \brief Returns this value as a complex number.
	\throw nothrow

	If the value instance does not represent a complex value the returned value
	is undefined. No exception is triggered. If you are unsure about the type
	use IsComplex() or GetType() to verify the type.
	*/
const cmplx_type& Value::GetComplex() const
{
	CheckType('c');
	return m_val;
}

//---------------------------------------------------------------------------
const string_type& Value::GetString() const
{
	CheckType('s');
	assert(data.m_psVal != nullptr);
	return *data.m_psVal;
}

//---------------------------------------------------------------------------
bool Value::GetBool() const
{
	CheckType('b');
	return data.m_bval;
}

//---------------------------------------------------------------------------
const matrix_type& Value::GetArray() const
{
	CheckType('m');
	assert(data.m_pvVal != nullptr);
	return *data.m_pvVal;
}

//---------------------------------------------------------------------------
int Value::GetRows() const
{
	return (GetType() != 'm') ? 1 : GetArray().GetRows();
}

//---------------------------------------------------------------------------
int Value::GetCols() const
{
	return (GetType() != 'm') ? 1 : GetArray().GetCols();
}

//---------------------------------------------------------------------------
void Value::CheckType(char_type a_cType) const
{
	if (m_cType != a_cType)
	{
		ErrorContext err;
		err.Errc = ecTYPE_CONFLICT;
		err.Type1 = m_cType;
		err.Type2 = a_cType;

		if (GetIdent().length())
		{
			err.Ident = GetIdent();
		}
		else
		{
			stringstream_type ss;
			ss << *this;
			err.Ident = ss.str();
		}

		throw ParserError(err);
	}
}

//---------------------------------------------------------------------------
bool Value::IsVariable() const
{
	return false;
}

//---------------------------------------------------------------------------
string_type Value::AsciiDump() const
{
	stringstream_type ss;

	ss << g_sCmdCode[GetCode()];
	ss << _T(" [addr=0x") << std::hex << this << std::dec;
	ss << _T("; pos=") << GetExprPos();
	ss << _T("; type=\"") << GetType() << _T("\"");
	ss << _T("; val=");

	switch (m_cType)
	{
	case 'i': ss << data.m_ival; break;
	case 'f': ss << data.m_fval; break;
	case 'm': ss << _T("(matrix)"); break;
	case 's':
		assert(data.m_psVal != nullptr);
		ss << _T("\"") << data.m_psVal << _T("\""); break;
	}

	ss << ((IsFlagSet(IToken::flVOLATILE)) ? _T("; ") : _T("; not ")) << _T("vol");
	ss << _T("]");

	return ss.str();
}

//-----------------------------------------------------------------------------------------------
void Value::Release()
{
	if (m_pCache)
		m_pCache->ReleaseToCache(this);
	else
		delete this;
}

//-----------------------------------------------------------------------------------------------
void Value::BindToCache(ValueCache* pCache)
{
	m_pCache = pCache;
}

//-----------------------------------------------------------------------------------------------
Value::operator cmplx_type ()
{
	return GetComplex();
}

//-----------------------------------------------------------------------------------------------
Value::operator int_type()
{
	return GetInteger();
}

//-----------------------------------------------------------------------------------------------
Value::operator string_type()
{
	return GetString();
}

//-----------------------------------------------------------------------------------------------
Value::operator float_type()
{
	return GetFloat();
}

//-----------------------------------------------------------------------------------------------
Value::operator bool()
{
	return GetBool();
}
}  // namespace mu
