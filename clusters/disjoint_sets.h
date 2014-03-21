#ifndef _DISJOINT_SETS_H_
#define _DISJOINT_SETS_H_

// A disjoint-set data structure

struct DisjointSetsNode
{
    friend class DisjointSets;

    DisjointSetsNode():
        disjointSetsParent(this),
        disjointSetsRank(0)
    {
    }

private:
    DisjointSetsNode* disjointSetsParent;
    int disjointSetsRank;
};


class DisjointSets
{
public:
    DisjointSets():
        m_setCount(0)
    {
    }

    int getSetCount() const
    {
        return m_setCount;
    }

    void makeSet(DisjointSetsNode* element)
    {
        element->disjointSetsParent = element;
        element->disjointSetsRank = 0;
        ++m_setCount;
    }

    void uniteSets(DisjointSetsNode* element1,
                   DisjointSetsNode* element2)
    {
        DisjointSetsNode* set1 = findSet(element1);
        DisjointSetsNode* set2 = findSet(element2);

        if (set1 != set2)
        {
            if (set1->disjointSetsRank > set2->disjointSetsRank)
            {
                set2->disjointSetsParent = set1;
            }
            else
            {
                set1->disjointSetsParent = set2;

                if (set1->disjointSetsRank == set2->disjointSetsRank)
                {
                    ++set2->disjointSetsRank;
                }
            }

            --m_setCount;
        }
    }

    bool sameComponent(DisjointSetsNode* element1,
                       DisjointSetsNode* element2)
    {
        return findSet(element1) == findSet(element2);
    }

    DisjointSetsNode* findSet(DisjointSetsNode* element)
    {
        if (element != element->disjointSetsParent)
        {
            element->disjointSetsParent = findSet(element->disjointSetsParent);
        }

        return element->disjointSetsParent;
    }

private:
    int m_setCount;
};

#endif
