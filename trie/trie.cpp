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

std::vector<std::string> trie::get_prefixes(const std::string& str) const{
    std::vector<std::string> blob;
    return blob;
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


trie::const_iterator& trie::const_iterator::operator++(){
    if(this->current_node == nullptr){
        return *this;
    }
    const trie_node *temp = this->current_node;
    trie_node *newTemp;
    bool foundDeep = false;
    bool foundElse = false;
    char prevPay = temp->payload;
    for (int i = 0; i < num_chars; ++i){
        if(temp->children[i] != nullptr){
            newTemp = temp->children[i];
            while(!newTemp->is_terminal){
                for (int j = 0; j < num_chars; ++j){
                    if(newTemp->children[j] != nullptr){
                        newTemp = newTemp->children[j];
                        break;
                    }
                }
            }
            foundDeep = true;
            break;
        }
    }
    if(!foundDeep){
        while(!foundElse && temp->parent != nullptr){
            prevPay = temp->payload;
            temp = temp->parent;
            for (int i = prevPay+1; i < num_chars; ++i){
                if(temp->children[i] != nullptr){
                    newTemp = temp->children[i];
                    while(!newTemp->is_terminal){
                        for (int i = 0; i < num_chars; ++i){
                            if(newTemp->children[i] != nullptr){
                                newTemp = newTemp->children[i];
                                break;
                            }
                        }
                    }
                    foundElse = true;
                    break;
                }
            }
        }
    }
    if(foundDeep || foundElse){
        this->current_node = newTemp;
        return *this;
    } else {
        this->current_node = nullptr;
        return *this;
    }
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