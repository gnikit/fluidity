// Wild Magic Source Code
// David Eberly
// http://www.geometrictools.com
// Copyright (c) 1998-2008
//
// This library is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 2.1 of the License, or (at
// your option) any later version.  The license is available for reading at
// either of the locations:
//     http://www.gnu.org/copyleft/lgpl.html
//     http://www.geometrictools.com/License/WildMagicLicense.pdf
//
// Version: 4.0.0 (2006/06/28)

//----------------------------------------------------------------------------
template <class TKEY>
THashSet<TKEY>::THashSet (int iTableSize)
{
    assert(iTableSize > 0);

    m_iTableSize = iTableSize;
    m_iQuantity = 0;
    m_iIndex = 0;
    m_pkItem = 0;
    m_apkTable = WM4_NEW HashItem*[m_iTableSize];
    memset(m_apkTable,0,m_iTableSize*sizeof(HashItem*));
    UserHashFunction = 0;
}
//----------------------------------------------------------------------------
template <class TKEY>
THashSet<TKEY>::~THashSet ()
{
    RemoveAll();
    WM4_DELETE[] m_apkTable;
}
//----------------------------------------------------------------------------
template <class TKEY>
int THashSet<TKEY>::GetQuantity () const
{
    return m_iQuantity;
}
//----------------------------------------------------------------------------
template <class TKEY>
TKEY* THashSet<TKEY>::Insert (const TKEY& rtKey)
{
    // find hash table entry for given key
    int iIndex = HashFunction(rtKey);
    HashItem* pkItem = m_apkTable[iIndex];

    // search for item in list associated with key
    while (pkItem)
    {
        if (rtKey == pkItem->m_tKey)
        {
            // item already in hash table
            return &pkItem->m_tKey;
        }
        pkItem = pkItem->m_pkNext;
    }

    // add item to beginning of list
    pkItem = WM4_NEW HashItem;
    pkItem->m_tKey = rtKey;
    pkItem->m_pkNext = m_apkTable[iIndex];
    m_apkTable[iIndex] = pkItem;
    m_iQuantity++;

    return &pkItem->m_tKey;
}
//----------------------------------------------------------------------------
template <class TKEY>
TKEY* THashSet<TKEY>::Get (const TKEY& rtKey) const
{
    // find hash table entry for given key
    int iIndex = HashFunction(rtKey);
    HashItem* pkItem = m_apkTable[iIndex];

    // search for item in list associated with key
    while (pkItem)
    {
        if (rtKey == pkItem->m_tKey)
        {
            // item is in hash table
            return &pkItem->m_tKey;
        }
        pkItem = pkItem->m_pkNext;
    }

    return 0;
}
//----------------------------------------------------------------------------
template <class TKEY>
bool THashSet<TKEY>::Remove (const TKEY& rtKey)
{
    // find hash table entry for given key
    int iIndex = HashFunction(rtKey);
    HashItem* pkItem = m_apkTable[iIndex];

    if (!pkItem)
    {
        return false;
    }

    if (rtKey == pkItem->m_tKey)
    {
        // item is at front of list, strip it off
        HashItem* pkSave = pkItem;
        m_apkTable[iIndex] = pkItem->m_pkNext;
        WM4_DELETE pkSave;
        m_iQuantity--;
        return true;
    }

    // search for item in list
    HashItem* pkPrev = pkItem;
    HashItem* pkCurr = pkItem->m_pkNext;
    while (pkCurr && rtKey != pkCurr->m_tKey)
    {
        pkPrev = pkCurr;
        pkCurr = pkCurr->m_pkNext;
    }

    if (pkCurr)
    {
        // found the item
        pkPrev->m_pkNext = pkCurr->m_pkNext;
        WM4_DELETE pkCurr;
        m_iQuantity--;
        return true;
    }

    return false;
}
//----------------------------------------------------------------------------
template <class TKEY>
void THashSet<TKEY>::RemoveAll ()
{
    if (m_iQuantity > 0)
    {
        for (int iIndex = 0; iIndex < m_iTableSize; iIndex++)
        {
            while (m_apkTable[iIndex])
            {
                HashItem* pkSave = m_apkTable[iIndex];
                m_apkTable[iIndex] = m_apkTable[iIndex]->m_pkNext;
                WM4_DELETE pkSave;
                if (--m_iQuantity == 0)
                {
                    return;
                }
            }
        }
    }
}
//----------------------------------------------------------------------------
template <class TKEY>
TKEY* THashSet<TKEY>::GetFirst () const
{
    if (m_iQuantity > 0)
    {
        for (m_iIndex = 0; m_iIndex < m_iTableSize; m_iIndex++)
        {
            if (m_apkTable[m_iIndex])
            {
                m_pkItem = m_apkTable[m_iIndex];
                return &m_pkItem->m_tKey;
            }
        }
    }

    return 0;
}
//----------------------------------------------------------------------------
template <class TKEY>
TKEY* THashSet<TKEY>::GetNext () const
{
    if (m_iQuantity > 0)
    {
        m_pkItem = m_pkItem->m_pkNext;
        if (m_pkItem)
        {
            return &m_pkItem->m_tKey;
        }

        for (m_iIndex++; m_iIndex < m_iTableSize; m_iIndex++)
        {
            if (m_apkTable[m_iIndex])
            {
                m_pkItem = m_apkTable[m_iIndex];
                return &m_pkItem->m_tKey;
            }
        }
    }

    return 0;
}
//----------------------------------------------------------------------------
template <class TKEY>
int THashSet<TKEY>::HashFunction (const TKEY& rtKey) const
{
    if (UserHashFunction)
    {
        return (*UserHashFunction)(rtKey);
    }

    // default hash function
    static double s_dHashMultiplier = 0.5*(sqrt(5.0)-1.0);
    unsigned int uiKey;
    System::Memcpy(&uiKey,sizeof(unsigned int),&rtKey,sizeof(unsigned int));
    uiKey %= m_iTableSize;
    double dFraction = fmod(s_dHashMultiplier*uiKey,1.0);
    return (int)floor(m_iTableSize*dFraction);
}
//----------------------------------------------------------------------------
