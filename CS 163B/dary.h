#pragma once
#ifndef DARY_H
#define DARY_H

#include <vector>
#include <map>
#include <cassert>

template <class T>
class dary_heap
{
public:

    dary_heap(std::size_t d = 2) : _d(d)
    {
        _n = 0;
    }

    std::size_t size() const
    {
        return _n;
    }

    T min() const
    {
        assert(_n != 0);
        return _data[0];
    }

    bool empty() const
    {
        return (_n == 0);
    }

    void push(const T& x)
    {

        assert(_l.count(x) == 0);   //  x is not already in heap
        if (_n == _data.size())     //  vector is full
            _data.push_back(x);
        else
            _data[_n] = x;


        // record where x lives in _data and increment the key count
        _l[x] = _n;
        ++_n;

        std::size_t i = _l.at(x);
        while (i > 0)
        {
            std::size_t parent = (i - 1) / _d;
            if (_data[i] < _data[parent])
            {
                swap(_data[i], _data[parent]);
                _l[_data[i]] = i;
                _l[_data[parent]] = parent;
                i = parent;

            }
            else
                break;
        }

    }

    void decrease_key(const T& x, const T& newx)
    {

        // new key is smaller than old key;  old key is in heap; new key is not yet in heap
        assert(newx < x&& _l.count(x) != 0 && _l.count(newx) == 0);

        std::size_t i = _l.at(x);
        _data[i] = newx;
        _l.erase(x);
        _l[newx] = i;


        while (i > 0 && _data[i] < _data[(i - 1) / _d])
        {
            std::size_t parent = (i - 1) / _d;

            swap(_data[i], _data[parent]);
            _l[_data[parent]] = parent;
            _l[_data[i]] = i;
            i = parent;


        }

    }


    void pop_min()
    {

        assert(!empty());

        _l.erase(_data[0]);
        if (--_n == 0)
            return;

        _data[0] = _data[_n];
        _l[_data[0]] = 0;
        std::size_t i = 0;

        while (i * _d + 1 < _n)      //  there is at least one child
        {
            std::size_t left(i * _d + 1), right((i + 1) * _d), m(left);

            for (std::size_t c = left; c < _n && c < right; ++c)
                if (_data[c] < _data[m])
                    m = c;


            // if smallest child is greater or equal to parent, then heap order has been restored
            if (!(_data[m] < _data[i]))
                break;

            // otherwise swap parent and minimum child
            swap(_data[i], _data[m]);
            _l[_data[i]] = i;
            _l[_data[m]] = m;
            i = m;
        }
    }

private:

    std::size_t _n;                   // number of elements in heap
    std::size_t _d;                   // number of children
    std::vector<T> _data;             // heap elements live here
    std::map<T, std::size_t> _l;      // _data[_l[key]] = key





};

#endif // DARY_H
