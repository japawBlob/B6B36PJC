#include "trie.hpp"

#include <utility>
#include <algorithm>
#include <cassert>
#include <iostream>

trie::trie() = default;
trie::trie(const std::vector<std::string>& strings){
    m_root = new trie_node;
    for (auto& str : strings) {
        this->insert(str);
    }
}

void free(trie_node* root){
    if(root != nullptr){
        for (auto child : root->children) {
            free(child);
        }
        delete root;
    }
}

trie::~trie(){
    free(m_root);
}

bool trie::contains(const std::string& str) const {
    if (m_root == nullptr) return false;
    trie_node* currentNode = m_root;

    for (int i = 0; i < str.size(); ++i) {
        if (currentNode->children[str[i]] != nullptr){
            currentNode = currentNode->children[str[i]];
        } else {
            return false;
        }
    }
    if (currentNode != nullptr && currentNode->is_terminal){
        return true;
    }
    return false;
}

bool trie::insert(const std::string& str){
    if (m_root == nullptr){
        m_root = new trie_node;
    }
    trie_node* currentNode = m_root;
    int i;
    for (i = 0; i < str.size(); ++i) {
        if (currentNode->children[str[i]] == nullptr) {
            currentNode->children[str[i]] = new trie_node;
            currentNode->children[str[i]]->payload = str[i];
            currentNode->children[str[i]]->parent = currentNode;
        }
        currentNode = currentNode->children[str[i]];
    }
    if(!currentNode->is_terminal){
        currentNode->is_terminal = true;
        m_size++;
        return true;
    }
    return false;

}

bool trie::erase(const std::string& str){
    if (m_root == nullptr){
        return false;
    }

    trie_node* currentNode = m_root;
    for (int i = 0; i < str.size(); ++i) {
        if (currentNode->children[str[i]] != nullptr){
            currentNode = currentNode->children[str[i]];
        } else {
            return false;
        }
    }
    if (currentNode->is_terminal){
        currentNode->is_terminal = false;
        while(currentNode->parent != nullptr) {
            for (auto child : currentNode->children) {
                if (child != nullptr) {
                    goto isEssential;
                }
            }
            auto temp = currentNode;
            currentNode = currentNode->parent;
            currentNode->children[temp->payload] = nullptr;
            delete temp;
        }
    } else {
        return false;
    }
    isEssential:
    m_size--;
    return true;
}

size_t trie::size() const {
    return m_size;
}

bool trie::empty() const {
    if(m_size == 0){
        return true;
    }
    return false;
}

/// STAGE 1 COMPLETE

trie::const_iterator::const_iterator(const trie_node* node){
    this->current_node = node;
}
trie::const_iterator trie::begin() const {
    trie_node* currentNode = m_root;
    if (m_root != nullptr){
        while(!currentNode->is_terminal) {
            for (auto child: currentNode->children) {
                if (child != nullptr) {
                    currentNode = child;
                    break;
                }
            }
        }
    }
    return trie::const_iterator(currentNode);
}
trie::const_iterator trie::end() const {
    return trie::const_iterator(nullptr);
}


trie::const_iterator& trie::const_iterator::operator++() {
    if(this->current_node == nullptr){
        return *this;
    }
    const trie_node *currentNode = this->current_node;
    trie_node *ret;
    bool foundDeep = false;
    bool foundElse = false;
    char prevPay = currentNode->payload;

    for (int i = prevPay; i < num_chars; ++i){
        if(currentNode->children[i] != nullptr){
            trie_node* child = currentNode->children[i];
            while(!child->is_terminal){
                for (int j = 0; j < num_chars; ++j){
                    if(child->children[j] != nullptr){
                        child = child->children[j];
                        break;
                    }
                }
            }
            ret = child;
            goto solutionFound;
        }
    }
    while(currentNode->parent != nullptr){
        prevPay = currentNode->payload;
        currentNode = currentNode->parent;
        for (int i = prevPay+1; i < num_chars; ++i){
            if(currentNode->children[i] != nullptr){
                trie_node* child = currentNode->children[i];
                while(!child->is_terminal){
                    for (int i = 0; i < num_chars; ++i){
                        if(child->children[i] != nullptr){
                            child = child->children[i];
                            break;
                        }
                    }
                }
                ret = child;
                goto solutionFound;
            }
        }
    }
    this->current_node = nullptr;
    return *this;

    solutionFound:
    this->current_node = ret;
    return *this;
}
trie::const_iterator trie::const_iterator::operator++(int){
    trie::const_iterator temp = *this;
    ++(*this);
    return temp;
}

trie::const_iterator::reference trie::const_iterator::operator*() const {
    std::string returnString;
    std::vector<char> tempBuffer;
    const trie_node* currentNode = current_node;
    while(currentNode->parent != nullptr){
        tempBuffer.push_back(currentNode->payload);
        currentNode = currentNode->parent;
    }
    for (int i = tempBuffer.size()-1; i >=0 ; i--) {
        returnString += tempBuffer[i];
    }
    return returnString;
}
bool trie::const_iterator::operator==(const const_iterator& rhs) const {
    return this->current_node == rhs.current_node;
}
bool trie::const_iterator::operator!=(const const_iterator& rhs) const{
    return this->current_node != rhs.current_node;
}

/// STAGE 2 complete

trie::trie(trie const& original){
    m_root = new trie_node();
    const_iterator iterator = original.begin();
    while(iterator != original.end()){
        insert(*iterator++);
    }
}
trie& trie::operator=(trie const& original){
    trie blob(original);
    this->swap (blob);
    return *this;
}
trie::trie(trie&& rhs) {
    swap(rhs);
}
trie& trie::operator=(trie&& rhs){
    swap(rhs);
    return *this;
}
void trie::swap(trie& rhs){
    trie_node* temp = rhs.m_root;
    rhs.m_root = m_root;
    m_root = temp;

    int blob = rhs.m_size;
    rhs.m_size = m_size;
    m_size = blob;
}
bool trie::operator<(const trie& rhs) const{
    /*if(rhs.m_size > m_size){
        return true;
    }*/

    const_iterator iteratorThis = this->begin();
    const_iterator iteratorRhs = rhs.begin();
    while(iteratorThis != this->end()){
        if (*iteratorThis > *iteratorRhs){
            return false;
        }
        if (*iteratorThis < *iteratorRhs){
            return true;
        }
        iteratorThis++;
        iteratorRhs++;
    }
    if(rhs.m_size > m_size){
        return true;
    }
    return false;
}
bool trie::operator==(const trie& rhs) const {
    if(this->m_size != rhs.m_size){
        return false;
    }
    const_iterator iteratorThis = this->begin();
    const_iterator iteratorRhs = this->begin();
    while(iteratorThis != this->end()){
        if (iteratorThis++ != iteratorRhs++){
            return false;
        }
    }
    return true;
}
bool operator!=(const trie& lhs, const trie& rhs){
    return !(lhs==rhs);
}
bool operator<=(const trie& lhs, const trie& rhs){
    return (lhs < rhs) || (lhs == rhs);
}
bool operator>(const trie& lhs, const trie& rhs){
    return !(lhs<=rhs);
}
bool operator>=(const trie& lhs, const trie& rhs){
    return (lhs > rhs) || (lhs == rhs);

}
std::ostream& operator<<(std::ostream& out, trie const& trie){
    out << "blobus";
    return out;
}
/// SEction 3 complete

std::vector<std::string> trie::search_by_prefix(const std::string& prefix) const {
    std::vector<std::string> ret;

    return ret;
}

std::vector<std::string> trie::get_prefixes(const std::string& str) const {
    std::vector<std::string> ret;
    trie_node* currentNode = this->m_root;
    if(currentNode == nullptr){
        return ret;
    }
    if(currentNode->is_terminal){
        ret.emplace_back("");
    }
    for (char c : str) {
        if(currentNode->children[c] != nullptr){
            if(currentNode->children[c]->is_terminal){
                const_iterator itr(currentNode->children[c]);
                ret.push_back(*itr);
            }
            currentNode = currentNode->children[c];
        } else {
            break;
        }
    }
    return ret;
}

trie trie::operator&(trie const& rhs) const {
    trie ret;
    const_iterator iterator = this->begin();
    while(iterator != this->end()){
        if (rhs.contains(*iterator) && this->contains(*iterator)){
            ret.insert(*iterator);
        }
        iterator++;
    }
    return ret;
}

trie trie::operator|(trie const& rhs) const {
    trie ret;

    /*const_iterator iteratorThis = this->begin();
    const_iterator iteratorRhs = rhs.begin();
    while( iteratorThis != this->end() || iteratorRhs != rhs.end() ){
        if(*iteratorRhs == *iteratorThis){
            ret.insert(*iteratorThis);
        } else {
            ret.insert(*iteratorThis);
            ret.insert(*iteratorRhs);
        }
        iteratorThis++;
        iteratorRhs++;
    }
    if(iteratorThis == this->end()){
        while(iteratorRhs != rhs.end()){
            ret.insert(*iteratorRhs);
            iteratorRhs++;
        }
    } else if(iteratorRhs == rhs.end()){
        while(iteratorThis != this->end()){
            ret.insert(*iteratorThis);
            iteratorThis++;
        }
    }*/
    const_iterator iteratorThis = this->begin();
    const_iterator iteratorRhs = rhs.begin();

    while (iteratorThis != this->end()){
        ret.insert(*iteratorThis);
        iteratorThis++;
    }
    while (iteratorRhs != rhs.end()){
        ret.insert(*iteratorRhs);
        iteratorRhs++;
    }

    return ret;
}