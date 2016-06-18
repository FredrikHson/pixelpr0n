#ifndef QUADTREE_HPP_
#define QUADTREE_HPP_

#include <stdio.h>

#include <vector>
#include <algorithm>

#define NUMSIDES 4

class rect
{
public:
    rect()
    {
        this->x = 0;
        this->y = 0;
        this->w = 0;
        this->h = 0;
    }
    rect(float x, float y, float w, float h)
    {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
    }
    virtual ~rect() {}

    float x;
    float y;
    float w;
    float h;
};

template <class T>
class Quadtree
{
public:
    Quadtree(float x, float y, float w, float h, int max_level,
             int level = 0, Quadtree *parent = nullptr)
    {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
        this->level = level;
        this->max_level = max_level;
        this->parent = parent;

        hW = (w * 0.5f) - 0.05f; // need this for floatingpoint error
        hH = (h * 0.5f) - 0.05f; // need this for floatingpoint error

        if(level != max_level) {
            sides[NW] = new Quadtree<T>(
                x,
                y,
                hW,
                hH,
                max_level,
                level + 1,
                this
            );
            sides[NE] = new Quadtree<T>(
                x + hW,
                y,
                hW,
                hH,
                max_level,
                level + 1,
                this
            );
            sides[SW] = new Quadtree<T>(
                x,
                y + hH,
                hW,
                hH,
                max_level,
                level + 1,
                this
            );
            sides[SE] = new Quadtree<T>(
                x + hW,
                y + hH,
                hW,
                hH,
                max_level,
                level + 1,
                this
            );
        }
    }

    virtual ~Quadtree()
    {
        if(level != max_level) {
            for(int i = 0; i < NUMSIDES; ++i) {
                delete sides[i];
            }
        }
    }

    void addObject(T *object)
    {
        if(level != max_level) {
            for(int i = 0; i < NUMSIDES; ++i) {
                if(contains(sides[i], object)) {
                    sides[i]->addObject(object);
                    return;
                }
            }
        }

        if(contains(this, object)) {
            objects.push_back(object);
        } else {
            if(parent != nullptr) {
                parent->addObject(object);
            } else {
                fprintf(
                    stderr,
                    "Out of bounds at %f,%f %fx%f!\n",
                    object->x,
                    object->y,
                    w,
                    h
                );
            }
        }
    }
    std::vector<T *> getObjectsAt(float x, float y)
    {
        if(level == max_level) {
            return objects;
        }

        std::vector<T *> ret_objects, temp;

        if(!objects.empty()) {
            ret_objects = objects;
        }

        if(x > this->x + hW && x < this->x + w) {
            if(y > this->y + hH && y < this->y + h) {
                temp = sides[SE]->getObjectsAt(x, y);
            } else if(y > this->y && y <= this->y + hH) {
                temp = sides[NE]->getObjectsAt(x, y);
            }
        } else if(x > this->x && x <= this->x + hW) {
            if(y > this->y + hH && y < this->y + h) {
                temp = sides[SW]->getObjectsAt(x, y);
            } else if(y > this->y && y <= this->y + hH) {
                temp = sides[NW]->getObjectsAt(x, y);
            }
        }

        if(temp.size() > 0) {
            ret_objects.insert(ret_objects.end(), temp.begin(), temp.end());
        }

        return ret_objects;
    }
    void clear()
    {
        if(level != max_level) {
            for(int i = 0; i < NUMSIDES; ++i) {
                sides[i]->clear();
            }
        }

        if(!objects.empty()) {
            objects.clear();
        }
    }
    bool hasObjects()
    {
        if(!objects.empty()) {
            return true;
        } else {
            if(level != max_level) {
                for(int i = 0; i < NUMSIDES; ++i) {
                    if(sides[i]->hasObjects()) {
                        return true;
                    }
                }
            }
        }

        return false;
    }
    std::vector<rect> visualize()
    {
        std::vector<rect> ret;

        ret.push_back(rect(x, y, w, h));

        if(level != max_level) {
            for(int i = 0; i < NUMSIDES; ++i) {
                if(sides[i]->hasObjects()) {
                    std::vector<rect> temp = sides[i]->visualize();

                    if(temp.size() > 0) {
                        ret.insert(ret.begin(), temp.begin(), temp.end());
                    }
                }
            }
        }

        return ret;
    }
    void update()
    {
        if(level != max_level) {
            for(int i = 0; i < NUMSIDES; ++i) {
                sides[i]->update();
            }
        }

        for(auto it = objects.begin(); it != objects.end();) {
            T *object = *it;

            if(!contains(this, object)) {
                it = objects.erase(it);
                addObject(object);
            } else {
                ++it;
            }
        }
    }
private:
    enum QUADSIDES { NW, NE, SW, SE };

    float x;
    float y;
    float w;
    float h;

    float hW; // Half w
    float hH; // Half h

    int level;
    int max_level;

    std::vector<T *> objects;

    Quadtree *parent;
    Quadtree *sides[4];

    bool contains(Quadtree *child, T *object)
    {
        return !(
                   object->x < child->x ||
                   object->y < child->y ||
                   object->x > child->x + child->w ||
                   object->y > child->y + child->h ||
                   object->x + object->w < child->x ||
                   object->y + object->h < child->y ||
                   object->x + object->w > child->x + child->w ||
                   object->y + object->h > child->y + child->h
               );
    }
};

#endif // QUADTREE_HPP_
