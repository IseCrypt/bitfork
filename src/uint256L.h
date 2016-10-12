// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_uint256L_H
#define BITCOIN_uint256L_H

#include <string>
#include <vector>

#include <stdint.h>
#include <stdio.h>
#include <string.h>

inline int Testuint256LAdHoc(std::vector<std::string> vArg);


/** Base class without constructors for uint256L and uint160L.
 * This makes the compiler let u use it in a union.
 */
template<unsigned int BITS>
class base_uintl
{
protected:
    enum { WIDTH=BITS/32 };
    unsigned int pn[WIDTH];
public:

    bool operator!() const
    {
        for (int i = 0; i < WIDTH; i++)
            if (pn[i] != 0)
                return false;
        return true;
    }

    const base_uintl operator~() const
    {
        base_uintl ret;
        for (int i = 0; i < WIDTH; i++)
            ret.pn[i] = ~pn[i];
        return ret;
    }

    const base_uintl operator-() const
    {
        base_uintl ret;
        for (int i = 0; i < WIDTH; i++)
            ret.pn[i] = ~pn[i];
        ret++;
        return ret;
    }

    double getdouble() const
    {
        double ret = 0.0;
        double fact = 1.0;
        for (int i = 0; i < WIDTH; i++) {
            ret += fact * pn[i];
            fact *= 4294967296.0;
        }
        return ret;
    }

    base_uintl& operator=(uint64_t b)
    {
        pn[0] = (unsigned int)b;
        pn[1] = (unsigned int)(b >> 32);
        for (int i = 2; i < WIDTH; i++)
            pn[i] = 0;
        return *this;
    }

    base_uintl& operator^=(const base_uintl& b)
    {
        for (int i = 0; i < WIDTH; i++)
            pn[i] ^= b.pn[i];
        return *this;
    }

    base_uintl& operator&=(const base_uintl& b)
    {
        for (int i = 0; i < WIDTH; i++)
            pn[i] &= b.pn[i];
        return *this;
    }

    base_uintl& operator|=(const base_uintl& b)
    {
        for (int i = 0; i < WIDTH; i++)
            pn[i] |= b.pn[i];
        return *this;
    }

    base_uintl& operator^=(uint64_t b)
    {
        pn[0] ^= (unsigned int)b;
        pn[1] ^= (unsigned int)(b >> 32);
        return *this;
    }

    base_uintl& operator|=(uint64_t b)
    {
        pn[0] |= (unsigned int)b;
        pn[1] |= (unsigned int)(b >> 32);
        return *this;
    }

    base_uintl& operator<<=(unsigned int shift)
    {
        base_uintl a(*this);
        for (int i = 0; i < WIDTH; i++)
            pn[i] = 0;
        int k = shift / 32;
        shift = shift % 32;
        for (int i = 0; i < WIDTH; i++)
        {
            if (i+k+1 < WIDTH && shift != 0)
                pn[i+k+1] |= (a.pn[i] >> (32-shift));
            if (i+k < WIDTH)
                pn[i+k] |= (a.pn[i] << shift);
        }
        return *this;
    }

    base_uintl& operator>>=(unsigned int shift)
    {
        base_uintl a(*this);
        for (int i = 0; i < WIDTH; i++)
            pn[i] = 0;
        int k = shift / 32;
        shift = shift % 32;
        for (int i = 0; i < WIDTH; i++)
        {
            if (i-k-1 >= 0 && shift != 0)
                pn[i-k-1] |= (a.pn[i] << (32-shift));
            if (i-k >= 0)
                pn[i-k] |= (a.pn[i] >> shift);
        }
        return *this;
    }

    base_uintl& operator+=(const base_uintl& b)
    {
        uint64_t carry = 0;
        for (int i = 0; i < WIDTH; i++)
        {
            uint64_t n = carry + pn[i] + b.pn[i];
            pn[i] = n & 0xffffffff;
            carry = n >> 32;
        }
        return *this;
    }

    base_uintl& operator-=(const base_uintl& b)
    {
        *this += -b;
        return *this;
    }

    base_uintl& operator+=(uint64_t b64)
    {
        base_uintl b;
        b = b64;
        *this += b;
        return *this;
    }

    base_uintl& operator-=(uint64_t b64)
    {
        base_uintl b;
        b = b64;
        *this += -b;
        return *this;
    }


    base_uintl& operator++()
    {
        // prefix operator
        int i = 0;
        while (++pn[i] == 0 && i < WIDTH-1)
            i++;
        return *this;
    }

    const base_uintl operator++(int)
    {
        // postfix operator
        const base_uintl ret = *this;
        ++(*this);
        return ret;
    }

    base_uintl& operator--()
    {
        // prefix operator
        int i = 0;
        while (--pn[i] == -1 && i < WIDTH-1)
            i++;
        return *this;
    }

    const base_uintl operator--(int)
    {
        // postfix operator
        const base_uintl ret = *this;
        --(*this);
        return ret;
    }


    friend inline bool operator<(const base_uintl& a, const base_uintl& b)
    {
        for (int i = base_uintl::WIDTH-1; i >= 0; i--)
        {
            if (a.pn[i] < b.pn[i])
                return true;
            else if (a.pn[i] > b.pn[i])
                return false;
        }
        return false;
    }

    friend inline bool operator<=(const base_uintl& a, const base_uintl& b)
    {
        for (int i = base_uintl::WIDTH-1; i >= 0; i--)
        {
            if (a.pn[i] < b.pn[i])
                return true;
            else if (a.pn[i] > b.pn[i])
                return false;
        }
        return true;
    }

    friend inline bool operator>(const base_uintl& a, const base_uintl& b)
    {
        for (int i = base_uintl::WIDTH-1; i >= 0; i--)
        {
            if (a.pn[i] > b.pn[i])
                return true;
            else if (a.pn[i] < b.pn[i])
                return false;
        }
        return false;
    }

    friend inline bool operator>=(const base_uintl& a, const base_uintl& b)
    {
        for (int i = base_uintl::WIDTH-1; i >= 0; i--)
        {
            if (a.pn[i] > b.pn[i])
                return true;
            else if (a.pn[i] < b.pn[i])
                return false;
        }
        return true;
    }

    friend inline bool operator==(const base_uintl& a, const base_uintl& b)
    {
        for (int i = 0; i < base_uintl::WIDTH; i++)
            if (a.pn[i] != b.pn[i])
                return false;
        return true;
    }

    friend inline bool operator==(const base_uintl& a, uint64_t b)
    {
        if (a.pn[0] != (unsigned int)b)
            return false;
        if (a.pn[1] != (unsigned int)(b >> 32))
            return false;
        for (int i = 2; i < base_uintl::WIDTH; i++)
            if (a.pn[i] != 0)
                return false;
        return true;
    }

    friend inline bool operator!=(const base_uintl& a, const base_uintl& b)
    {
        return (!(a == b));
    }

    friend inline bool operator!=(const base_uintl& a, uint64_t b)
    {
        return (!(a == b));
    }



    std::string GetHex() const
    {
        char psz[sizeof(pn)*2 + 1];
        for (unsigned int i = 0; i < sizeof(pn); i++)
            sprintf(psz + i*2, "%02x", ((unsigned char*)pn)[sizeof(pn) - i - 1]);
        return std::string(psz, psz + sizeof(pn)*2);
    }

    void SetHex(const char* psz)
    {
        for (int i = 0; i < WIDTH; i++)
            pn[i] = 0;

        // skip leading spaces
        while (isspace(*psz))
            psz++;

        // skip 0x
        if (psz[0] == '0' && tolower(psz[1]) == 'x')
            psz += 2;

        // hex string to uint
        static const unsigned char phexdigit[256] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,1,2,3,4,5,6,7,8,9,0,0,0,0,0,0, 0,0xa,0xb,0xc,0xd,0xe,0xf,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0xa,0xb,0xc,0xd,0xe,0xf,0,0,0,0,0,0,0,0,0 };
        const char* pbegin = psz;
        while (phexdigit[(unsigned char)*psz] || *psz == '0')
            psz++;
        psz--;
        unsigned char* p1 = (unsigned char*)pn;
        unsigned char* pend = p1 + WIDTH * 4;
        while (psz >= pbegin && p1 < pend)
        {
            *p1 = phexdigit[(unsigned char)*psz--];
            if (psz >= pbegin)
            {
                *p1 |= (phexdigit[(unsigned char)*psz--] << 4);
                p1++;
            }
        }
    }

    void SetHex(const std::string& str)
    {
        SetHex(str.c_str());
    }

    std::string ToString() const
    {
        return (GetHex());
    }

    unsigned char* begin()
    {
        return (unsigned char*)&pn[0];
    }

    unsigned char* end()
    {
        return (unsigned char*)&pn[WIDTH];
    }

    unsigned int size()
    {
        return sizeof(pn);
    }

    uint64_t Get64(int n=0) const
    {
        return pn[2*n] | (uint64_t)pn[2*n+1] << 32;
    }

    unsigned int GetSerializeSize(int nType, int nVersion) const
    {
        return sizeof(pn);
    }

    template<typename Stream>
    void Serialize(Stream& s, int nType, int nVersion) const
    {
        s.write((char*)pn, sizeof(pn));
    }

    template<typename Stream>
    void Unserialize(Stream& s, int nType, int nVersion)
    {
        s.read((char*)pn, sizeof(pn));
    }

    friend class uint160L;
    friend class uint256L;
	friend class uint512L;
    friend inline int Testuint256LAdHoc(std::vector<std::string> vArg);
};

typedef base_uintl<160> base_uintl160L;
typedef base_uintl<256> base_uintl256L;
typedef base_uintl<512> base_uintl512L;

//
// uint160L and uint256L could be implemented as templates, but to keep
// compile errors and debugging cleaner, they're copy and pasted.
//



//////////////////////////////////////////////////////////////////////////////
//
// uint160L
//

/** 160-bit unsigned integer */
class uint160L : public base_uintl160L
{
public:
    typedef base_uintl160L basetype;

    uint160L()
    {
        for (int i = 0; i < WIDTH; i++)
            pn[i] = 0;
    }

    uint160L(const basetype& b)
    {
        for (int i = 0; i < WIDTH; i++)
            pn[i] = b.pn[i];
    }

    uint160L& operator=(const basetype& b)
    {
        for (int i = 0; i < WIDTH; i++)
            pn[i] = b.pn[i];
        return *this;
    }

    uint160L(uint64_t b)
    {
        pn[0] = (unsigned int)b;
        pn[1] = (unsigned int)(b >> 32);
        for (int i = 2; i < WIDTH; i++)
            pn[i] = 0;
    }

    uint160L& operator=(uint64_t b)
    {
        pn[0] = (unsigned int)b;
        pn[1] = (unsigned int)(b >> 32);
        for (int i = 2; i < WIDTH; i++)
            pn[i] = 0;
        return *this;
    }

    explicit uint160L(const std::string& str)
    {
        SetHex(str);
    }

    explicit uint160L(const std::vector<unsigned char>& vch)
    {
        if (vch.size() == sizeof(pn))
            memcpy(pn, &vch[0], sizeof(pn));
        else
            *this = 0;
    }
};

inline bool operator==(const uint160L& a, uint64_t b)                         { return (base_uintl160L)a == b; }
inline bool operator!=(const uint160L& a, uint64_t b)                         { return (base_uintl160L)a != b; }
inline const uint160L operator<<(const base_uintl160L& a, unsigned int shift)   { return uint160L(a) <<= shift; }
inline const uint160L operator>>(const base_uintl160L& a, unsigned int shift)   { return uint160L(a) >>= shift; }
inline const uint160L operator<<(const uint160L& a, unsigned int shift)        { return uint160L(a) <<= shift; }
inline const uint160L operator>>(const uint160L& a, unsigned int shift)        { return uint160L(a) >>= shift; }

inline const uint160L operator^(const base_uintl160L& a, const base_uintl160L& b) { return uint160L(a) ^= b; }
inline const uint160L operator&(const base_uintl160L& a, const base_uintl160L& b) { return uint160L(a) &= b; }
inline const uint160L operator|(const base_uintl160L& a, const base_uintl160L& b) { return uint160L(a) |= b; }
inline const uint160L operator+(const base_uintl160L& a, const base_uintl160L& b) { return uint160L(a) += b; }
inline const uint160L operator-(const base_uintl160L& a, const base_uintl160L& b) { return uint160L(a) -= b; }

inline bool operator<(const base_uintl160L& a, const uint160L& b)          { return (base_uintl160L)a <  (base_uintl160L)b; }
inline bool operator<=(const base_uintl160L& a, const uint160L& b)         { return (base_uintl160L)a <= (base_uintl160L)b; }
inline bool operator>(const base_uintl160L& a, const uint160L& b)          { return (base_uintl160L)a >  (base_uintl160L)b; }
inline bool operator>=(const base_uintl160L& a, const uint160L& b)         { return (base_uintl160L)a >= (base_uintl160L)b; }
inline bool operator==(const base_uintl160L& a, const uint160L& b)         { return (base_uintl160L)a == (base_uintl160L)b; }
inline bool operator!=(const base_uintl160L& a, const uint160L& b)         { return (base_uintl160L)a != (base_uintl160L)b; }
inline const uint160L operator^(const base_uintl160L& a, const uint160L& b) { return (base_uintl160L)a ^  (base_uintl160L)b; }
inline const uint160L operator&(const base_uintl160L& a, const uint160L& b) { return (base_uintl160L)a &  (base_uintl160L)b; }
inline const uint160L operator|(const base_uintl160L& a, const uint160L& b) { return (base_uintl160L)a |  (base_uintl160L)b; }
inline const uint160L operator+(const base_uintl160L& a, const uint160L& b) { return (base_uintl160L)a +  (base_uintl160L)b; }
inline const uint160L operator-(const base_uintl160L& a, const uint160L& b) { return (base_uintl160L)a -  (base_uintl160L)b; }

inline bool operator<(const uint160L& a, const base_uintl160L& b)          { return (base_uintl160L)a <  (base_uintl160L)b; }
inline bool operator<=(const uint160L& a, const base_uintl160L& b)         { return (base_uintl160L)a <= (base_uintl160L)b; }
inline bool operator>(const uint160L& a, const base_uintl160L& b)          { return (base_uintl160L)a >  (base_uintl160L)b; }
inline bool operator>=(const uint160L& a, const base_uintl160L& b)         { return (base_uintl160L)a >= (base_uintl160L)b; }
inline bool operator==(const uint160L& a, const base_uintl160L& b)         { return (base_uintl160L)a == (base_uintl160L)b; }
inline bool operator!=(const uint160L& a, const base_uintl160L& b)         { return (base_uintl160L)a != (base_uintl160L)b; }
inline const uint160L operator^(const uint160L& a, const base_uintl160L& b) { return (base_uintl160L)a ^  (base_uintl160L)b; }
inline const uint160L operator&(const uint160L& a, const base_uintl160L& b) { return (base_uintl160L)a &  (base_uintl160L)b; }
inline const uint160L operator|(const uint160L& a, const base_uintl160L& b) { return (base_uintl160L)a |  (base_uintl160L)b; }
inline const uint160L operator+(const uint160L& a, const base_uintl160L& b) { return (base_uintl160L)a +  (base_uintl160L)b; }
inline const uint160L operator-(const uint160L& a, const base_uintl160L& b) { return (base_uintl160L)a -  (base_uintl160L)b; }

inline bool operator<(const uint160L& a, const uint160L& b)               { return (base_uintl160L)a <  (base_uintl160L)b; }
inline bool operator<=(const uint160L& a, const uint160L& b)              { return (base_uintl160L)a <= (base_uintl160L)b; }
inline bool operator>(const uint160L& a, const uint160L& b)               { return (base_uintl160L)a >  (base_uintl160L)b; }
inline bool operator>=(const uint160L& a, const uint160L& b)              { return (base_uintl160L)a >= (base_uintl160L)b; }
inline bool operator==(const uint160L& a, const uint160L& b)              { return (base_uintl160L)a == (base_uintl160L)b; }
inline bool operator!=(const uint160L& a, const uint160L& b)              { return (base_uintl160L)a != (base_uintl160L)b; }
inline const uint160L operator^(const uint160L& a, const uint160L& b)      { return (base_uintl160L)a ^  (base_uintl160L)b; }
inline const uint160L operator&(const uint160L& a, const uint160L& b)      { return (base_uintl160L)a &  (base_uintl160L)b; }
inline const uint160L operator|(const uint160L& a, const uint160L& b)      { return (base_uintl160L)a |  (base_uintl160L)b; }
inline const uint160L operator+(const uint160L& a, const uint160L& b)      { return (base_uintl160L)a +  (base_uintl160L)b; }
inline const uint160L operator-(const uint160L& a, const uint160L& b)      { return (base_uintl160L)a -  (base_uintl160L)b; }






//////////////////////////////////////////////////////////////////////////////
//
// uint256L
//

/** 256-bit unsigned integer */
class uint256L : public base_uintl256L
{
public:
    typedef base_uintl256L basetype;

    uint256L()
    {
        for (int i = 0; i < WIDTH; i++)
            pn[i] = 0;
    }

    uint256L(const basetype& b)
    {
        for (int i = 0; i < WIDTH; i++)
            pn[i] = b.pn[i];
    }

    uint256L& operator=(const basetype& b)
    {
        for (int i = 0; i < WIDTH; i++)
            pn[i] = b.pn[i];
        return *this;
    }

    uint256L(uint64_t b)
    {
        pn[0] = (unsigned int)b;
        pn[1] = (unsigned int)(b >> 32);
        for (int i = 2; i < WIDTH; i++)
            pn[i] = 0;
    }

    uint256L& operator=(uint64_t b)
    {
        pn[0] = (unsigned int)b;
        pn[1] = (unsigned int)(b >> 32);
        for (int i = 2; i < WIDTH; i++)
            pn[i] = 0;
        return *this;
    }

    explicit uint256L(const std::string& str)
    {
        SetHex(str);
    }

    explicit uint256L(const std::vector<unsigned char>& vch)
    {
        if (vch.size() == sizeof(pn))
            memcpy(pn, &vch[0], sizeof(pn));
        else
            *this = 0;
    }
};

inline bool operator==(const uint256L& a, uint64_t b)                         { return (base_uintl256L)a == b; }
inline bool operator!=(const uint256L& a, uint64_t b)                         { return (base_uintl256L)a != b; }
inline const uint256L operator<<(const base_uintl256L& a, unsigned int shift)   { return uint256L(a) <<= shift; }
inline const uint256L operator>>(const base_uintl256L& a, unsigned int shift)   { return uint256L(a) >>= shift; }
inline const uint256L operator<<(const uint256L& a, unsigned int shift)        { return uint256L(a) <<= shift; }
inline const uint256L operator>>(const uint256L& a, unsigned int shift)        { return uint256L(a) >>= shift; }

inline const uint256L operator^(const base_uintl256L& a, const base_uintl256L& b) { return uint256L(a) ^= b; }
inline const uint256L operator&(const base_uintl256L& a, const base_uintl256L& b) { return uint256L(a) &= b; }
inline const uint256L operator|(const base_uintl256L& a, const base_uintl256L& b) { return uint256L(a) |= b; }
inline const uint256L operator+(const base_uintl256L& a, const base_uintl256L& b) { return uint256L(a) += b; }
inline const uint256L operator-(const base_uintl256L& a, const base_uintl256L& b) { return uint256L(a) -= b; }

inline bool operator<(const base_uintl256L& a, const uint256L& b)          { return (base_uintl256L)a <  (base_uintl256L)b; }
inline bool operator<=(const base_uintl256L& a, const uint256L& b)         { return (base_uintl256L)a <= (base_uintl256L)b; }
inline bool operator>(const base_uintl256L& a, const uint256L& b)          { return (base_uintl256L)a >  (base_uintl256L)b; }
inline bool operator>=(const base_uintl256L& a, const uint256L& b)         { return (base_uintl256L)a >= (base_uintl256L)b; }
inline bool operator==(const base_uintl256L& a, const uint256L& b)         { return (base_uintl256L)a == (base_uintl256L)b; }
inline bool operator!=(const base_uintl256L& a, const uint256L& b)         { return (base_uintl256L)a != (base_uintl256L)b; }
inline const uint256L operator^(const base_uintl256L& a, const uint256L& b) { return (base_uintl256L)a ^  (base_uintl256L)b; }
inline const uint256L operator&(const base_uintl256L& a, const uint256L& b) { return (base_uintl256L)a &  (base_uintl256L)b; }
inline const uint256L operator|(const base_uintl256L& a, const uint256L& b) { return (base_uintl256L)a |  (base_uintl256L)b; }
inline const uint256L operator+(const base_uintl256L& a, const uint256L& b) { return (base_uintl256L)a +  (base_uintl256L)b; }
inline const uint256L operator-(const base_uintl256L& a, const uint256L& b) { return (base_uintl256L)a -  (base_uintl256L)b; }

inline bool operator<(const uint256L& a, const base_uintl256L& b)          { return (base_uintl256L)a <  (base_uintl256L)b; }
inline bool operator<=(const uint256L& a, const base_uintl256L& b)         { return (base_uintl256L)a <= (base_uintl256L)b; }
inline bool operator>(const uint256L& a, const base_uintl256L& b)          { return (base_uintl256L)a >  (base_uintl256L)b; }
inline bool operator>=(const uint256L& a, const base_uintl256L& b)         { return (base_uintl256L)a >= (base_uintl256L)b; }
inline bool operator==(const uint256L& a, const base_uintl256L& b)         { return (base_uintl256L)a == (base_uintl256L)b; }
inline bool operator!=(const uint256L& a, const base_uintl256L& b)         { return (base_uintl256L)a != (base_uintl256L)b; }
inline const uint256L operator^(const uint256L& a, const base_uintl256L& b) { return (base_uintl256L)a ^  (base_uintl256L)b; }
inline const uint256L operator&(const uint256L& a, const base_uintl256L& b) { return (base_uintl256L)a &  (base_uintl256L)b; }
inline const uint256L operator|(const uint256L& a, const base_uintl256L& b) { return (base_uintl256L)a |  (base_uintl256L)b; }
inline const uint256L operator+(const uint256L& a, const base_uintl256L& b) { return (base_uintl256L)a +  (base_uintl256L)b; }
inline const uint256L operator-(const uint256L& a, const base_uintl256L& b) { return (base_uintl256L)a -  (base_uintl256L)b; }

inline bool operator<(const uint256L& a, const uint256L& b)               { return (base_uintl256L)a <  (base_uintl256L)b; }
inline bool operator<=(const uint256L& a, const uint256L& b)              { return (base_uintl256L)a <= (base_uintl256L)b; }
inline bool operator>(const uint256L& a, const uint256L& b)               { return (base_uintl256L)a >  (base_uintl256L)b; }
inline bool operator>=(const uint256L& a, const uint256L& b)              { return (base_uintl256L)a >= (base_uintl256L)b; }
inline bool operator==(const uint256L& a, const uint256L& b)              { return (base_uintl256L)a == (base_uintl256L)b; }
inline bool operator!=(const uint256L& a, const uint256L& b)              { return (base_uintl256L)a != (base_uintl256L)b; }
inline const uint256L operator^(const uint256L& a, const uint256L& b)      { return (base_uintl256L)a ^  (base_uintl256L)b; }
inline const uint256L operator&(const uint256L& a, const uint256L& b)      { return (base_uintl256L)a &  (base_uintl256L)b; }
inline const uint256L operator|(const uint256L& a, const uint256L& b)      { return (base_uintl256L)a |  (base_uintl256L)b; }
inline const uint256L operator+(const uint256L& a, const uint256L& b)      { return (base_uintl256L)a +  (base_uintl256L)b; }
inline const uint256L operator-(const uint256L& a, const uint256L& b)      { return (base_uintl256L)a -  (base_uintl256L)b; }





//////////////////////////////////////////////////////////////////////////////
//
// uint512L
//

/** 512-bit unsigned integer */
class uint512L : public base_uintl512L
{
public:
    typedef base_uintl512L basetype;

    uint512L()
    {
        for (int i = 0; i < WIDTH; i++)
            pn[i] = 0;
    }

    uint512L(const basetype& b)
    {
        for (int i = 0; i < WIDTH; i++)
            pn[i] = b.pn[i];
    }

    uint512L& operator=(const basetype& b)
    {
        for (int i = 0; i < WIDTH; i++)
            pn[i] = b.pn[i];
        return *this;
    }

    uint512L(uint64_t b)
    {
        pn[0] = (unsigned int)b;
        pn[1] = (unsigned int)(b >> 32);
        for (int i = 2; i < WIDTH; i++)
            pn[i] = 0;
    }

    uint512L& operator=(uint64_t b)
    {
        pn[0] = (unsigned int)b;
        pn[1] = (unsigned int)(b >> 32);
        for (int i = 2; i < WIDTH; i++)
            pn[i] = 0;
        return *this;
    }

    explicit uint512L(const std::string& str)
    {
        SetHex(str);
    }

    explicit uint512L(const std::vector<unsigned char>& vch)
    {
        if (vch.size() == sizeof(pn))
            memcpy(pn, &vch[0], sizeof(pn));
        else
            *this = 0;
    }

    uint256L trim256() const
    {
        uint256L ret;
        for (unsigned int i = 0; i < uint256L::WIDTH; i++){
            ret.pn[i] = pn[i];
        }
        return ret;
    }
};

inline bool operator==(const uint512L& a, uint64_t b)                           { return (base_uintl512L)a == b; }
inline bool operator!=(const uint512L& a, uint64_t b)                           { return (base_uintl512L)a != b; }
inline const uint512L operator<<(const base_uintl512L& a, unsigned int shift)   { return uint512L(a) <<= shift; }
inline const uint512L operator>>(const base_uintl512L& a, unsigned int shift)   { return uint512L(a) >>= shift; }
inline const uint512L operator<<(const uint512L& a, unsigned int shift)        { return uint512L(a) <<= shift; }
inline const uint512L operator>>(const uint512L& a, unsigned int shift)        { return uint512L(a) >>= shift; }

inline const uint512L operator^(const base_uintl512L& a, const base_uintl512L& b) { return uint512L(a) ^= b; }
inline const uint512L operator&(const base_uintl512L& a, const base_uintl512L& b) { return uint512L(a) &= b; }
inline const uint512L operator|(const base_uintl512L& a, const base_uintl512L& b) { return uint512L(a) |= b; }
inline const uint512L operator+(const base_uintl512L& a, const base_uintl512L& b) { return uint512L(a) += b; }
inline const uint512L operator-(const base_uintl512L& a, const base_uintl512L& b) { return uint512L(a) -= b; }

inline bool operator<(const base_uintl512L& a, const uint512L& b)          { return (base_uintl512L)a <  (base_uintl512L)b; }
inline bool operator<=(const base_uintl512L& a, const uint512L& b)         { return (base_uintl512L)a <= (base_uintl512L)b; }
inline bool operator>(const base_uintl512L& a, const uint512L& b)          { return (base_uintl512L)a >  (base_uintl512L)b; }
inline bool operator>=(const base_uintl512L& a, const uint512L& b)         { return (base_uintl512L)a >= (base_uintl512L)b; }
inline bool operator==(const base_uintl512L& a, const uint512L& b)         { return (base_uintl512L)a == (base_uintl512L)b; }
inline bool operator!=(const base_uintl512L& a, const uint512L& b)         { return (base_uintl512L)a != (base_uintl512L)b; }
inline const uint512L operator^(const base_uintl512L& a, const uint512L& b) { return (base_uintl512L)a ^  (base_uintl512L)b; }
inline const uint512L operator&(const base_uintl512L& a, const uint512L& b) { return (base_uintl512L)a &  (base_uintl512L)b; }
inline const uint512L operator|(const base_uintl512L& a, const uint512L& b) { return (base_uintl512L)a |  (base_uintl512L)b; }
inline const uint512L operator+(const base_uintl512L& a, const uint512L& b) { return (base_uintl512L)a +  (base_uintl512L)b; }
inline const uint512L operator-(const base_uintl512L& a, const uint512L& b) { return (base_uintl512L)a -  (base_uintl512L)b; }

inline bool operator<(const uint512L& a, const base_uintl512L& b)          { return (base_uintl512L)a <  (base_uintl512L)b; }
inline bool operator<=(const uint512L& a, const base_uintl512L& b)         { return (base_uintl512L)a <= (base_uintl512L)b; }
inline bool operator>(const uint512L& a, const base_uintl512L& b)          { return (base_uintl512L)a >  (base_uintl512L)b; }
inline bool operator>=(const uint512L& a, const base_uintl512L& b)         { return (base_uintl512L)a >= (base_uintl512L)b; }
inline bool operator==(const uint512L& a, const base_uintl512L& b)         { return (base_uintl512L)a == (base_uintl512L)b; }
inline bool operator!=(const uint512L& a, const base_uintl512L& b)         { return (base_uintl512L)a != (base_uintl512L)b; }
inline const uint512L operator^(const uint512L& a, const base_uintl512L& b) { return (base_uintl512L)a ^  (base_uintl512L)b; }
inline const uint512L operator&(const uint512L& a, const base_uintl512L& b) { return (base_uintl512L)a &  (base_uintl512L)b; }
inline const uint512L operator|(const uint512L& a, const base_uintl512L& b) { return (base_uintl512L)a |  (base_uintl512L)b; }
inline const uint512L operator+(const uint512L& a, const base_uintl512L& b) { return (base_uintl512L)a +  (base_uintl512L)b; }
inline const uint512L operator-(const uint512L& a, const base_uintl512L& b) { return (base_uintl512L)a -  (base_uintl512L)b; }

inline bool operator<(const uint512L& a, const uint512L& b)               { return (base_uintl512L)a <  (base_uintl512L)b; }
inline bool operator<=(const uint512L& a, const uint512L& b)              { return (base_uintl512L)a <= (base_uintl512L)b; }
inline bool operator>(const uint512L& a, const uint512L& b)               { return (base_uintl512L)a >  (base_uintl512L)b; }
inline bool operator>=(const uint512L& a, const uint512L& b)              { return (base_uintl512L)a >= (base_uintl512L)b; }
inline bool operator==(const uint512L& a, const uint512L& b)              { return (base_uintl512L)a == (base_uintl512L)b; }
inline bool operator!=(const uint512L& a, const uint512L& b)              { return (base_uintl512L)a != (base_uintl512L)b; }
inline const uint512L operator^(const uint512L& a, const uint512L& b)      { return (base_uintl512L)a ^  (base_uintl512L)b; }
inline const uint512L operator&(const uint512L& a, const uint512L& b)      { return (base_uintl512L)a &  (base_uintl512L)b; }
inline const uint512L operator|(const uint512L& a, const uint512L& b)      { return (base_uintl512L)a |  (base_uintl512L)b; }
inline const uint512L operator+(const uint512L& a, const uint512L& b)      { return (base_uintl512L)a +  (base_uintl512L)b; }
inline const uint512L operator-(const uint512L& a, const uint512L& b)      { return (base_uintl512L)a -  (base_uintl512L)b; }







#ifdef TEST_uint256L

inline int Testuint256LAdHoc(std::vector<std::string> vArg)
{
    uint256L g(0);


    printf("%s\n", g.ToString().c_str());
    g--;  printf("g--\n");
    printf("%s\n", g.ToString().c_str());
    g--;  printf("g--\n");
    printf("%s\n", g.ToString().c_str());
    g++;  printf("g++\n");
    printf("%s\n", g.ToString().c_str());
    g++;  printf("g++\n");
    printf("%s\n", g.ToString().c_str());
    g++;  printf("g++\n");
    printf("%s\n", g.ToString().c_str());
    g++;  printf("g++\n");
    printf("%s\n", g.ToString().c_str());



    uint256L a(7);
    printf("a=7\n");
    printf("%s\n", a.ToString().c_str());

    uint256L b;
    printf("b undefined\n");
    printf("%s\n", b.ToString().c_str());
    int c = 3;

    a = c;
    a.pn[3] = 15;
    printf("%s\n", a.ToString().c_str());
    uint256L k(c);

    a = 5;
    a.pn[3] = 15;
    printf("%s\n", a.ToString().c_str());
    b = 1;
    b <<= 52;

    a |= b;

    a ^= 0x500;

    printf("a %s\n", a.ToString().c_str());

    a = a | b | (uint256L)0x1000;


    printf("a %s\n", a.ToString().c_str());
    printf("b %s\n", b.ToString().c_str());

    a = 0xfffffffe;
    a.pn[4] = 9;

    printf("%s\n", a.ToString().c_str());
    a++;
    printf("%s\n", a.ToString().c_str());
    a++;
    printf("%s\n", a.ToString().c_str());
    a++;
    printf("%s\n", a.ToString().c_str());
    a++;
    printf("%s\n", a.ToString().c_str());

    a--;
    printf("%s\n", a.ToString().c_str());
    a--;
    printf("%s\n", a.ToString().c_str());
    a--;
    printf("%s\n", a.ToString().c_str());
    uint256L d = a--;
    printf("%s\n", d.ToString().c_str());
    printf("%s\n", a.ToString().c_str());
    a--;
    printf("%s\n", a.ToString().c_str());
    a--;
    printf("%s\n", a.ToString().c_str());

    d = a;

    printf("%s\n", d.ToString().c_str());
    for (int i = uint256L::WIDTH-1; i >= 0; i--) printf("%08x", d.pn[i]); printf("\n");

    uint256L neg = d;
    neg = ~neg;
    printf("%s\n", neg.ToString().c_str());


    uint256L e = uint256L("0xABCDEF123abcdef12345678909832180000011111111");
    printf("\n");
    printf("%s\n", e.ToString().c_str());


    printf("\n");
    uint256L x1 = uint256L("0xABCDEF123abcdef12345678909832180000011111111");
    uint256L x2;
    printf("%s\n", x1.ToString().c_str());
    for (int i = 0; i < 270; i += 4)
    {
        x2 = x1 << i;
        printf("%s\n", x2.ToString().c_str());
    }

    printf("\n");
    printf("%s\n", x1.ToString().c_str());
    for (int i = 0; i < 270; i += 4)
    {
        x2 = x1;
        x2 >>= i;
        printf("%s\n", x2.ToString().c_str());
    }


    for (int i = 0; i < 100; i++)
    {
        uint256L k = (~uint256L(0) >> i);
        printf("%s\n", k.ToString().c_str());
    }

    for (int i = 0; i < 100; i++)
    {
        uint256L k = (~uint256L(0) << i);
        printf("%s\n", k.ToString().c_str());
    }

    return (0);
}

#endif

#endif
