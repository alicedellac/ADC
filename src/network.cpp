#include "network.h"
#include "random.h"


void Network::resize(const size_t& newSize) {
	if (values.size() == newSize) return;
	
	if (values.size() > newSize) {
		//deleting extra entries
		for (size_t i(values.size()); i > newSize; --i) {
			values.pop_back();
		}
	} else { 
		//adding missing entries
		for (size_t i(values.size()); i < newSize; ++i) {
			values.push_back(RNG.normal());
		}
	}
}

bool Network::add_link(const size_t& a, const size_t& b) {
	if (a == b) return false;

	if (a >= values.size() or b >= values.size()) return false;
	
	if (is_linked(a, b)) return true; //a and b are already linked
		
	//link a to b:
	links.insert(std::pair<size_t, size_t> (a, b));
	//link b to a:
	links.insert(std::pair<size_t, size_t> (b, a));
	return true;
}

size_t Network::random_connect(const double& mean_deg) {
		
	//clear all previous links
	for (size_t i(0); i < values.size(); ++i) { 
		if (degree(i) > 0) links.erase(i);
	}
	
	for (size_t i(0); i < values.size(); ++i) {
		
		//how many new links for node i (poisson)
		size_t nbLinks = RNG.poisson(mean_deg); 
		
		for (size_t j(0); j < nbLinks; ++j) {
			add_link(RNG.uniform_double(0, values.size()), i);			
		}
	}
	
	return links.size()/2;
}

size_t Network::set_values(const std::vector<double>& vec) {
	
	values.clear();
	values = vec;
	
	return values.size();
}

size_t Network::size() const {

	return values.size();
}

size_t Network::degree(const size_t &_n) const {
	size_t compteur(0);
	
	for (std::pair<size_t, size_t> element : links) {
		if (element.first == _n) ++compteur; 
	}
	return compteur;
}

double Network::value(const size_t &_n) const {
	
	return values[_n];
}
	
std::vector<double> Network::sorted_values() const {
	std::vector<double> copied_values = values;
	std::sort(copied_values.begin(), copied_values.end(), std::greater<double>());
	return copied_values;
}

std::vector<size_t> Network::neighbors(const size_t& a) const {
	std::vector<size_t> neighbor;
	
	if (links.find(a) != links.end()) { //a is a key
		for (std::pair<size_t, size_t> element : links) {
			if (element.first == a) neighbor.push_back(element.second); 
		}
		return neighbor;
	}
	
	neighbor = {0};
	return neighbor ;
}

bool Network::is_linked(const size_t& a, const size_t& b) const {

	for (std::pair<size_t, size_t> element : links) {
		if (element.first == a and element.second == b) return true;
	}

	return false;
}

