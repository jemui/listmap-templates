// $Id: listmap.tcc,v 1.11 2018-01-25 14:19:14-08 - - $
// Jeanette Mui, jemui@ucsc.edu

#include "listmap.h"
#include "debug.h"

//
/////////////////////////////////////////////////////////////////
// Operations on listmap::node.
/////////////////////////////////////////////////////////////////
//

//
// listmap::node::node (link*, link*, const value_type&)
//
template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::node::node (node* next, node* prev,
                                     const value_type& value):
            link (next, prev), value (value) {
}

//
/////////////////////////////////////////////////////////////////
// Operations on listmap.
/////////////////////////////////////////////////////////////////
//

//
// listmap::~listmap()
//
template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::~listmap() {
   DEBUGF ('l', reinterpret_cast<const void*> (this));

   while(anchor_.next != anchor_.prev) {
      node* old = anchor_.next; 
      anchor_.next = anchor_.next->next;
      delete old; 
   }
}

//
// iterator listmap::insert (const value_type&)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::insert (const value_type& pair) {
   DEBUGF ('l', &pair << "->" << pair);

   // get copy of list pointers
   node** curr = &list.next;

   // insert into empty list
   if(list.next == nullptr) {
      *curr = new node(next, prev, pair);

      // head and prev points at itself 
      (*curr)->next = (*curr)->prev = (*curr);
      start = (*curr);

      // list stores pointer to new node
      list = link((*curr)->next, (*curr)->prev);
      anchor_ = list;
      
      cout << (*curr)->value.first << " = " << (*curr)->value.second <<endl;

      return iterator(); 
   }

   bool is_equal = not less((*curr)->value.first, pair.first) and 
                  not less(pair.first, (*curr)->value.first);

   if(is_equal) {
      (*curr)->value.second = pair.second;
   } 
   else if(less(pair.first, (*curr)->value.first)) {
      node* new_node = new node((*curr), (*curr)->prev, pair);

      (*curr)->prev = new_node;
      list = link((*curr), (*curr)->prev);

      // move to lowest value
      (*curr) = (*curr)->prev;
      cout << (*curr)->value.first << " = " << (*curr)->value.second <<endl;
   }
   else {
      node* lowest = (*curr);
      while((*curr) != start->next and less((*curr)->value.first, pair.first)) {
         (*curr) = (*curr)->prev;
      }

      node* new_node = new node((*curr)->next, (*curr), pair); 

      (*curr)->next = new_node;
      (*curr)->prev = new_node;

      list = link((*curr)->next, (*curr)->prev);

      cout << (*curr)->value.first << " = " << (*curr)->value.second <<endl;
      (*curr) = lowest;
   }

   anchor_ = list; 
   return iterator();
}

//
// listmap::find(const key_type&)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::find (const key_type& that) {
   DEBUGF ('l', that);

   // get pointer to list
   node** curr = &list.prev;
   bool is_equal = not less((*curr)->value.first, that) and 
                  not less(that, (*curr)->value.first);
 
   // searh through list
   while(not is_equal) {
      (*curr) = (*curr)->next; 
      is_equal = not less((*curr)->value.first, that) and 
                  not less(that, (*curr)->value.first);
   } 

   // key either exists or does not exist
   if(is_equal)
      cout << (*curr)->value.first << " = " << (*curr)->value.second << endl;
   else 
      cerr << (*curr)->value.first << ": key not found" << endl; 

   return iterator();
}

//
// iterator listmap::erase (iterator position)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::erase (iterator position) {
   DEBUGF ('l', &*position);

   position = nullptr;

   return iterator();
}


//
/////////////////////////////////////////////////////////////////
// Operations on listmap::iterator.
/////////////////////////////////////////////////////////////////
//

//
// listmap::value_type& listmap::iterator::operator*()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::value_type&
listmap<Key,Value,Less>::iterator::operator*() {
   DEBUGF ('l', where);
   return where->value;
}

//
// listmap::value_type* listmap::iterator::operator->()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::value_type*
listmap<Key,Value,Less>::iterator::operator->() {
   DEBUGF ('l', where);
   return &(where->value);
}

//
// listmap::iterator& listmap::iterator::operator++()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator&
listmap<Key,Value,Less>::iterator::operator++() {
   DEBUGF ('l', where);
   where = where->next;
   return *this;
}

//
// listmap::iterator& listmap::iterator::operator--()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator&
listmap<Key,Value,Less>::iterator::operator--() {
   DEBUGF ('l', where);
   where = where->prev;
   return *this;
}


//
// bool listmap::iterator::operator== (const iterator&)
//
template <typename Key, typename Value, class Less>
inline bool listmap<Key,Value,Less>::iterator::operator==
            (const iterator& that) const {
   return this->where == that.where;
}

//
// bool listmap::iterator::operator!= (const iterator&)
//
template <typename Key, typename Value, class Less>
inline bool listmap<Key,Value,Less>::iterator::operator!=
            (const iterator& that) const {
   return this->where != that.where;
}

