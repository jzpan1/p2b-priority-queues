// Project identifier: 43DE0E0C4C76BFAA6D8C2F5AEAE0518A9C42CF4E

#ifndef PAIRINGPQ_H
#define PAIRINGPQ_H

#include <deque>
#include <utility>

#ifdef DEBUG
    #include <iostream>
#endif

#include "Eecs281PQ.hpp"

// A specialized version of the priority queue ADT implemented as a pairing
// heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class PairingPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Each node within the pairing heap
    class Node {
    public:
        // Description: Custom constructor that creates a node containing
        //              the given value.
        explicit Node(const TYPE &val)
            : elt { val } {}

        // Description: Allows access to the element at that Node's position.
        //              There are two versions, getElt() and a dereference
        //              operator, use whichever one seems more natural to you.
        // Runtime: O(1) - this has been provided for you.
        const TYPE &getElt() const { return elt; }
        const TYPE &operator*() const { return elt; }

        // The following line allows you to access any private data
        // members of this Node class from within the PairingPQ class.
        // (ie: myNode.elt is a legal statement in PairingPQ's add_node()
        // function).
        friend PairingPQ;

    private:
        TYPE elt;
        Node *child = nullptr;
        Node *sibling = nullptr;
        Node *parent = nullptr;
    };  // Node


    // Description: Construct an empty pairing heap with an optional
    //              comparison functor.
    // Runtime: O(1)
    explicit PairingPQ(COMP_FUNCTOR comp = COMP_FUNCTOR())
        : BaseClass { comp }, count(0), root(nullptr)  {
        // TODO: Implement this function.
        #ifdef DEBUG
            std::cout << "default constructor...\n";
        #endif 
    }  // PairingPQ()


    // Description: Construct a pairing heap out of an iterator range with an
    //              optional comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    template<typename InputIterator>
    PairingPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR())
        : BaseClass { comp }, count(0), root(nullptr) {
        // TODO: Implement this function.
        #ifdef DEBUG
            std::cout << "iterator constructor...\n";
        #endif 
        for (InputIterator i = start; i != end; i++) {
            push(*i);
            #ifdef DEBUG
                std::cout << "size " << count << '\n' ;
            #endif
        }
        #ifdef DEBUG
            std::cout << "root address: " << root << '\n';
        #endif 
    }  // PairingPQ()


    // Description: Copy constructor.
    // Runtime: O(n)
    PairingPQ(const PairingPQ &other)
        : BaseClass { other.compare }, count(0), root(nullptr) {
        // TODO: Implement this function.
        #ifdef DEBUG
            std::cout << "copy constructor...\n";
        #endif 
        
        std::deque<Node*> dq;
        dq.push_back(other.root);
        while(!dq.empty()) {
            Node* p = dq.front();
            dq.pop_front();
            push(p->elt);
            if (p->child) dq.push_back(p->child);
            if (p->sibling) dq.push_back(p->sibling);
            #ifdef DEBUG
                std::cout << "size: " << count << '\n';
            #endif 
        }
    }  // PairingPQ()


    // Description: Copy assignment operator.
    // Runtime: O(n)
    PairingPQ &operator=(const PairingPQ &rhs) {
        // TODO: Implement this function.
        #ifdef DEBUG
            std::cout << "= operator...\n";
        #endif 
        PairingPQ temp(rhs);
        std::swap(count, temp.count);
        std::swap(root, temp.root);

        return *this;
    }  // operator=()


    // Description: Destructor
    // Runtime: O(n)
    ~PairingPQ() {
        // TODO: Implement this function.
        #ifdef DEBUG
            std::cout << "destructing...\n";
        #endif
        if (count == 0) return;
        std::deque<Node*> dq {root};
        while(!dq.empty()) {
            Node* p = dq.front();
            #ifdef DEBUG
                std::cout << "deleting " << p << '\n';
            #endif
            dq.pop_front();
            if (p->child) dq.push_back(p->child);
            if (p->sibling) dq.push_back(p->sibling);
            delete p;
        }
    }  // ~PairingPQ()


    // Description: Move constructor and assignment operators don't need any
    //              code, the members will be reused automatically.
    PairingPQ(PairingPQ &&) noexcept = default;
    PairingPQ &operator=(PairingPQ &&) noexcept = default;


    // Description: Assumes that all elements inside the pairing heap are out
    //              of order and 'rebuilds' the pairing heap by fixing the
    //              pairing heap invariant.  You CANNOT delete 'old' nodes
    //              and create new ones!
    // Runtime: O(n)
    virtual void updatePriorities() {
        // TODO: Implement this function.std::deque<Node*> dq {root};
        #ifdef DEBUG
            std::cout << "update priorities...\n";
        #endif
        if (count < 2) return;
        std::deque<Node*> dq {root->child};
        root->child = nullptr;
        while(!dq.empty()) {
            Node* p = dq.front();
            #ifdef DEBUG
                std::cout << "updating " << p << '\n';
            #endif
            dq.pop_front();
            if (p->child) dq.push_back(p->child);
            if (p->sibling) dq.push_back(p->sibling);
            p->parent = nullptr;
            p->sibling = nullptr;
            p->child = nullptr;
            root = meld(root, p);
        }
    }  // updatePriorities()


    // Description: Add a new element to the pairing heap. This is already
    //              done. You should implement push functionality entirely
    //              in the addNode() function, and this function calls
    //              addNode().
    // Runtime: O(1)
    virtual void push(const TYPE &val) { addNode(val); }  // push()


    // Description: Remove the most extreme (defined by 'compare') element
    //              from the pairing heap.
    // Note: We will not run tests on your code that would require it to pop
    // an element when the pairing heap is empty. Though you are welcome to
    // if you are familiar with them, you do not need to use exceptions in
    // this project.
    // Runtime: Amortized O(log(n))
    virtual void pop() {
        // TODO: Implement this function.
        #ifdef DEBUG
            std::cout << "pop...\n";
        #endif 
        Node* p = root->child;
        delete root;
        if (p == nullptr) {
            count = 0;
            root = nullptr;
            return;
        }
        std::deque<Node*> dq;
        do {
            p->parent = nullptr;
            dq.push_back(p);
        } while ((p=p->sibling)!= nullptr);
        while (dq.size()!=1) {
            Node* p1 = dq.front();
            p1->sibling = nullptr;
            dq.pop_front();
            Node* p2 = dq.front();
            p2->sibling = nullptr;
            dq.pop_front();
            dq.push_back(meld(p1, p2));
        }
        root = dq.front();
        count--;
        #ifdef DEBUG
            std::cout << "size: " << size() 
                      << ", root address: " << root << '\n';
        #endif 
    }  // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the pairing heap. This should be a reference for speed.
    //              It MUST be const because we cannot allow it to be
    //              modified, as that might make it no longer be the most
    //              extreme element.
    // Runtime: O(1)
    virtual const TYPE &top() const {
        // TODO: Implement this function
        return root->elt;
    }  // top()


    // Description: Get the number of elements in the pairing heap.
    // Runtime: O(1)
    [[nodiscard]] virtual std::size_t size() const {
        // TODO: Implement this function
        return count;
    }  // size()

    // Description: Return true if the pairing heap is empty.
    // Runtime: O(1)
    [[nodiscard]] virtual bool empty() const {
        // TODO: Implement this function
        return count == 0;  // TODO: Delete or change this line
    }  // empty()


    // Description: Updates the priority of an element already in the pairing
    //              heap by replacing the element refered to by the Node with
    //              new_value.  Must maintain pairing heap invariants.
    //
    // PRECONDITION: The new priority, given by 'new_value' must be more
    //              extreme (as defined by comp) than the old priority.
    //
    // Runtime: As discussed in reading material.
    void updateElt(Node *node, const TYPE &new_value) {
        // TODO: Implement this function
        #ifdef DEBUG
            std::cout << "update elt...\n";
        #endif 
        node->elt = new_value;
        if (node->parent) {
            Node* p = node->parent->child;
            if (p==node) {
                node->parent->child = node->sibling;
            }
            else {
                while (p->sibling!=node) {
                    p = p->sibling;
                }
                p->sibling = node->sibling;
            }
        }
        node->sibling = nullptr;
        node->parent = nullptr;
        root = meld(root, node);
    }  // updateElt()


    // Description: Add a new element to the pairing heap. Returns a Node*
    //              corresponding to the newly added element.
    // Runtime: O(1)
    // NOTE: Whenever you create a node, and thus return a Node *, you must
    //       be sure to never move or copy/delete that node in the future,
    //       until it is eliminated by the user calling pop(). Remember this
    //       when you implement updateElt() and updatePriorities().
    Node *addNode(const TYPE &val) {
        // TODO: Implement this function
        #ifdef DEBUG
            std::cout << "add node...\n";
        #endif 
        Node* newNode = new Node(val);
        #ifdef DEBUG
            std::cout << "node address " << newNode <<'\n';
        #endif 
        
        if (count == 0) {
            root = newNode;
        }
        else {
            root = meld(root, newNode);
        }
        count++;
        return newNode;
    }  // addNode()


private:
    // TODO: Add any additional member variables or member functions you
    // require here.
    // TODO: We recommend creating a 'meld' function (see the Pairing Heap
    // papers).
    Node* meld(Node *pa, Node *pb) {
        #ifdef DEBUG
            if (pa->parent!=nullptr) throw std::invalid_argument("258");
            if (pb->parent!=nullptr) throw std::invalid_argument("259");
            if (pa->sibling!=nullptr) throw std::invalid_argument("260");
            if (pb->sibling!=nullptr) throw std::invalid_argument("261");
        #endif
        if (this->compare(pa->elt, pb->elt)) {
            pa->parent = pb;
            pa->sibling = pb->child;
            pb->child = pa;
            return pb;
        }
        else {
            pb->parent = pa;
            pb->sibling = pa->child;
            pa->child = pb;
            return pa;
        }
    }
    size_t count;
    Node *root;
    // NOTE: For member variables, you are only allowed to add a "root
    //       pointer" and a "count" of the number of nodes. Anything else
    //       (such as a deque) should be declared inside of member functions
    //       as needed.
};


#endif  // PAIRINGPQ_H
