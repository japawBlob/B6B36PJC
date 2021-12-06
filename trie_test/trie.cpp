#include "trie.hpp"

#include <utility>
#include <algorithm>
#include <cassert>
#include <iostream>

/**
 * Základní konstruktor
 */
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

///****************************///****************************///****************************///****************************

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

/**
 * Posune iterátor na abecedně následující řetězec v trie
 */
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


///*********************************************************************************************************************


/**

 * Operátor výpisu do proudu.
 *
 * Tuto funkci netestujeme, ale pokud ji vhodně implementujete, budete mít
 * ve výstupech z testů užitěčně vypsaný obsah trie.
 */
std::ostream& operator<<(std::ostream& out, trie const& trie)
{
    //std::cout << "";
    return out;
}


/**
 * Prohodí všechny prvky mezi touto trií a `rhs`.
 */
void trie::swap(trie& rhs) {
    trie_node *rootyTooty = rhs.m_root;
    rhs.m_root = m_root;
    m_root = rootyTooty;

    int temSize = rhs.m_size;
    rhs.m_size = m_size;
    m_size = temSize;
}

/**
 * ADL customization point pro std::swap.
 * Výsledek `swap(lhs, rhs);` by měl být ekvivalentní s výsledkem
 * `lhs.swap(rhs);`
 */
void swap(trie& lhs, trie& rhs) {
    lhs.swap(rhs);
}

/**
 * Zkopíruje všechny řetězce z rhs do dané trie (this)
 */
trie::trie(const trie& rhs) {
    m_root = new trie_node();
    for (const_iterator itr = rhs.begin(); itr != rhs.end(); itr++) {
        insert(*itr);
    }
}

/**
 * Zkopíruje všechny řetězce z rhs do dané trie (this), tak aby zmizeli včechny řetězce co byli v původní trii (this)
 */
trie& trie::operator=(const trie& rhs) {
    trie new_trie(rhs);
    swap(new_trie);
    return *this;
}

/**
 * Přesune všechny řetězce z rhs do dané trie (this)
 */
trie::trie(trie&& rhs) {
    swap(rhs);
}

/**
 * Přesune všechny řetězce z rhs do dané trie (this)
 */
trie& trie::operator=(trie&& rhs) {
    swap(rhs);
    return *this;
}

/**
 * Vrací `true` pokud je trie `rhs` roven této.
 */
bool trie::operator==(const trie& rhs) const {
    if (m_size != rhs.m_size)
    {
        return false;
    }

    const_iterator itr2 = begin();
    for (const_iterator itr = rhs.begin(); itr != rhs.end(); itr++) {
        if(*itr != *itr2){
            return false;
        }
        itr2++;
    }
    return true;
}

/**
 * Vrací `true` pokud je tato trie menší než `rhs`.
 */
bool trie::operator<(const trie& rhs) const {
    for (const_iterator itr = rhs.begin(), itr2 = begin(); itr != rhs.end() && itr2 != end(); itr++, itr2++) {
        if(*itr > *itr2){
            return true;
        } else if (*itr < *itr2){
            return false;
        }
    }

    if(rhs.m_size > m_size){
        return true;
    }

    return false;
}

/**
 * 2 trie jsou si nerovné právě tehdy, když si nejsou rovné (viz operator==)
 */
bool operator!=(const trie& lhs, const trie& rhs) {
    return !(lhs == rhs);
}

/**
 * Lexicografické uspořádání, viz operator<
 */
bool operator<=(const trie& lhs, const trie& rhs) {
    return lhs == rhs || lhs < rhs;
}

/**
 * Lexicografické uspořádání, viz operator<
 */
bool operator>(const trie& lhs, const trie& rhs) {
    return !(lhs <= rhs);
}

/**
 * Lexicografické uspořádání, viz operator<
 */
bool operator>=(const trie& lhs, const trie& rhs) {
    return lhs == rhs || lhs > rhs;
}


/**
 * Vrátí všechny prvky trie, které začínají daným prefixem.
 */
std::vector<std::string> trie::search_by_prefix(const std::string& prefix) const {
    std::vector<std::string> res;
    if(m_root == nullptr){
        return res;
    }
    const_iterator itr = begin();
    while(*itr < prefix){
        itr++;
        if (itr == end())
        {
            return res;
        }
    }
    while((*itr).find(prefix) == 0)
    {
        res.push_back(*itr);
        itr++;
        if (itr == end())
        {
            return res;
        }
    }

    return res;
}

/**
 * Vrátí všechny řetězce z trie, jež jsou prefixem daného řetězce.
 */
std::vector<std::string> trie::get_prefixes(const std::string& str) const {
    std::vector<std::string> res;
    if(m_root == nullptr){
        return res;
    } else if (m_root->is_terminal)
    {
        res.push_back("");
    }
    trie_node* tempParent = m_root;

    for (int i = 0; i < str.size(); ++i)
    {
        if (tempParent->children[str[i]] != nullptr)
        {
            tempParent = tempParent->children[str[i]];
            if (tempParent->is_terminal)
            {
                const_iterator itr(tempParent);
                res.push_back(*itr);
            }
        } else{
            return res;
        }
    }
    return res;
}

/**
 * Vrátí novou trii, která obsahuje průnik této a `rhs`
 */
trie trie::operator&(trie const& rhs) const {
    trie res;
    for (const_iterator itr = this->begin(); itr != this->end(); itr++) {
        if(rhs.contains(*itr) && this->contains(*itr)){
            res.insert(*itr);
        }
    }

    return res;
}

/**
 * Vrátí novou trii, která obsahuje sjednocení této a `rhs`
 */
trie trie::operator|(trie const& rhs) const {
    trie res;
    for (const_iterator itr = this->begin(); itr != this->end(); itr++) {
        res.insert(*itr);
    }

    for (const_iterator itr = rhs.begin(); itr != rhs.end(); itr++) {
        res.insert(*itr);
    }
    return res;
}


/*int main()
{
	trie trie({ "abcd"});
	if (trie.contains("abcd"))
	{
		std::cout << "abcd je in \n";
	}
	if (!trie.contains(""))
	{
		std::cout << "  neni in \n";
	}
	if (!trie.contains("a"))
	{
		std::cout << " a neni in \n";
	}
	if (!trie.contains("ab"))
	{
		std::cout << " ab neni in \n";
	}
	if (!trie.contains("abc"))
	{
		std::cout << " abc neni in \n";
	}
	trie.insert("abc");
	if (trie.contains("abc"))
	{
		std::cout << "abc je in \n";
	}





}*/
