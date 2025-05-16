#pragma once

template <typename implementing_class>
class IndexAble{
    virtual int getID() const = 0;
    virtual ~IndexAble() = default;
};