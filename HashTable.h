//
//  HashTable.h
//  CS 32 Project 4
//
//  Created by Andy Yu on 3/8/15.
//  Copyright (c) 2015 Hongseok (Andy) Yu. All rights reserved.
//

#ifndef __CS_32_Project_4__HashTable__
#define __CS_32_Project_4__HashTable__

#include <stdio.h>
#include <iostream>
using namespace std;

template <typename K, typename V>
class Node;

template <typename KeyType,	typename ValueType>

class HashTable
{
public:
    HashTable(unsigned int numBuckets,	unsigned int capacity);
    ~HashTable();
    bool isFull()	const;
    bool set(const KeyType&	key,	const ValueType& value, bool permanent =	false);
    bool get(const KeyType& key,	ValueType& value)	const;
    bool touch(const KeyType& key);
    bool discard(KeyType& key,	ValueType& value);
private:
				//	We	prevent a	HashTable from	being	copied	or	assigned	by	declaring	the
				//	copy	constructor	and	assignment	operator	private	and	not	implementing	them.
    HashTable(const HashTable&);
    HashTable& operator=(const HashTable&);
    Node <KeyType, ValueType> **hashtable;
    Node <KeyType, ValueType> *newest;
    Node <KeyType, ValueType> *oldest;
    int numnodes;
    int numbuckets;
    int cap;
    unsigned int getBucketForKey(const KeyType& key) const;
};

template <typename K>
unsigned int computeHash(K key);

template <typename K, typename V>
class Node
{
public:
    Node(const K &key, const V &value, bool perm){
        m_key = key;
        m_value = value;
        prev = nullptr;
        next = nullptr;
        younger = nullptr;
        older = nullptr;
        permanent = perm;
    }
    
    Node(){
        prev = nullptr;
        next = nullptr;
        younger = nullptr;
        older = nullptr;
    }
    
    K getKey() const {
        return m_key;
    }
    
    void setValue(V value) {
        m_value = value;
    }
    
    V getValue() const {
        return m_value;
    }
    
    void setPrev(Node *p) {
        prev = p;
    }
    
    Node *getPrev() const {
        return prev;
    }
    
    void setNext(Node *n) {
        next = n;
    }
    
    Node *getNext() const {
        return next;
    }
    
    void setYounger(Node*n) {
        younger = n;
    }
    
    Node *getYounger() const {
        return younger;
    }
    
    void setOlder(Node *n) {
        older = n;
    }
    
    Node *getOlder() const {
        return older;
    }
    
    bool isPermanent() {
        return permanent;
    }
    
private:
    K m_key;
    V m_value;
    Node *prev;
    Node *next;
    Node *younger;
    Node *older;
    bool permanent;
};

template <typename KeyType,	typename ValueType>
HashTable <KeyType, ValueType>::HashTable(unsigned int numBuckets, unsigned int capacity)
{
    hashtable = new Node <KeyType, ValueType>* [numBuckets];
    for (int i = 0; i < numBuckets; i++)
        hashtable[i] = nullptr;
    numnodes = 0;
    cap = capacity;
    numbuckets = numBuckets;
    newest = nullptr;
    oldest = nullptr;
}

template <typename KeyType,	typename ValueType>
HashTable <KeyType, ValueType>::~HashTable()
{
    for (int i = 0; i < numbuckets; ++i)
    {
        Node<KeyType, ValueType> *curr = hashtable[i];
        while (curr != nullptr) {
            Node<KeyType, ValueType> *temp = curr;
            curr = curr->getNext();
            delete temp;
        }
        hashtable[i] = nullptr;
    }
    delete [] hashtable;
}

template <typename KeyType,	typename ValueType>
bool HashTable <KeyType, ValueType>::isFull() const
{
    if (numnodes >= cap)
        return true;
    return false;
}

template <typename KeyType,	typename ValueType>
bool HashTable <KeyType, ValueType>::set(const KeyType&	key, const ValueType& value, bool permanent)
{
    if (isFull())
        return false;
    unsigned int hash = getBucketForKey(key);
    Node<KeyType, ValueType> *prev = nullptr;
    Node<KeyType, ValueType> *curr = hashtable[hash];
    Node<KeyType, ValueType> *entry = new Node<KeyType, ValueType>(key, value, permanent);
    while (curr != nullptr && curr->getKey() != key)
    {
        prev = curr;
        curr = curr->getNext();
    }
    if (curr == nullptr)
    {
        if (prev == nullptr)
            hashtable[hash] = entry;
        else
        {
            prev->setNext(entry);
            entry->setPrev(prev);
        }
    }
    else if (curr->getKey() == key)
    {
        if (curr->getYounger() != nullptr) // check if curr is the youngest node
        {
            if (curr->getOlder() != nullptr) {// check if curr is the oldest node
                curr->getYounger()->setOlder(curr->getOlder());
                curr->getOlder()->setYounger(curr->getYounger());
            }
            else
                curr->getYounger()->setOlder(nullptr); // if curr is oldest
            Node<KeyType, ValueType> *temp = newest;
            newest = curr;
            curr->setOlder(temp);
            curr->setYounger(nullptr);
            temp->setYounger(newest);
        }
        curr->setValue(value);
        return true;
    }
    if (!permanent)
    {
        if (oldest == nullptr)
            oldest = entry;
        if (newest == nullptr)
            newest = entry;
        else
        {
            Node<KeyType, ValueType> *temp = newest;
            newest = entry;
            entry->setOlder(temp);
            temp->setYounger(entry);
        }
    }
    numnodes++;
    return true;
}

template <typename KeyType,	typename ValueType>
bool HashTable <KeyType, ValueType>::get(const KeyType& key, ValueType& value) const
{
    unsigned int hash = getBucketForKey(key);
    Node<KeyType, ValueType> *curr = hashtable[hash];
    while (curr != nullptr && curr->getKey() != key)
    {
        curr = curr->getNext();
    }
    if (curr!= nullptr && curr->getKey() == key)
    {
        value  = curr->getValue();
        return true;
    }
    return false;
}

template <typename KeyType,	typename ValueType>
bool HashTable <KeyType, ValueType>::touch(const KeyType& key)
{
    unsigned int hash = getBucketForKey(key);                                                
    Node<KeyType, ValueType> *curr = hashtable[hash];
    while (curr != nullptr && curr->getKey() != key)
    {
        curr = curr->getNext();
    }
    //cout << curr->getKey() << endl;
    if (curr != nullptr && curr->getKey() == key)
    {
        if (curr->isPermanent())
            return false;
        else
        {
            //cout << key << endl;
            if (curr->getYounger() != nullptr) // check if curr is the youngest node
            {
                if (curr->getOlder() != nullptr) {// check if curr is the oldest node
                    //cout << key << endl;
                    curr->getYounger()->setOlder(curr->getOlder());
                    curr->getOlder()->setYounger(curr->getYounger());
                }
                else
                {
                    curr->getYounger()->setOlder(nullptr); // if curr is oldest
                    oldest = curr->getYounger();
                }
                Node<KeyType, ValueType> *temp = newest;
                newest = curr;
                curr->setOlder(temp);
                curr->setYounger(nullptr);
                temp->setYounger(newest);
            }
        }
    }
    else
        return false;
    return true;
}

template <typename KeyType,	typename ValueType>
bool HashTable <KeyType, ValueType>::discard(KeyType& key,	ValueType& value)
{
    if (newest==nullptr)
        return false;
    else
    {
        Node<KeyType, ValueType> *curr = oldest;
        curr->getYounger()->setOlder(nullptr);
        if (curr->getPrev() == nullptr)
        {
            curr->getNext()->setPrev(nullptr);
            hashtable[getBucketForKey(curr->getKey())] = curr->getNext();
        }
        else if (curr->getNext() == nullptr)
            curr->getPrev()->setNext(nullptr);
        else
        {
            curr->getPrev()->setNext(curr->getNext());
            curr->getNext()->setPrev(curr->getPrev());
        }
        oldest = curr->getYounger();
        key = curr->getKey();
        value = curr->getValue();
        delete curr;
        curr = nullptr;
    }
    return true;
}

template <typename KeyType,	typename ValueType>
unsigned int HashTable <KeyType, ValueType>::getBucketForKey(const KeyType& key) const
{
    unsigned int computeHash(KeyType);
    unsigned int result = computeHash(key);
    return result % numbuckets;
}

#endif /* defined(__CS_32_Project_4__HashTable__) */
