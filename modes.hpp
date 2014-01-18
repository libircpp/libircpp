#ifndef IRC_MODES_HPP
#define IRC_MODES_HPP

#include "types.hpp"

#include <ostream>
#include <vector>

namespace irc {

class mode_block {
public:
	using value_type    =std::pair<char, optional_string>;
	using iterator      =std::vector<value_type>::iterator;
	using const_iterator=std::vector<value_type>::const_iterator;

	void insert(char sym);
	void insert(char sym, const std::string& param);
	void erase(char sym);

	const_iterator begin() const;
	const_iterator end() const;

	iterator       find(char sym);
	const_iterator find(char sym) const;

	bool is_mode_set(char sym) const;

	void set_mode(char sym, const optional_string& param);
	void set_mode(const std::vector<value_type>& modes);
	void unset_mode(char sym);
	void unset_mode(std::vector<char> syms);
	optional_string try_get_mode_param(char sym);
private:
	void set_mode_impl(char sym, const optional_string& param);
	void unset_mode_impl(char sym);

	using container=std::vector<value_type>;

	sig_vc    on_unset_mode;
	sig_vcos  on_set_mode;
	container modes;
}; //class modes_block

std::ostream& operator<<(std::ostream& os, const mode_block& mb);

std::string to_string(const mode_block& mb);


using mode_list=std::vector<mode_block::value_type>;
using act_mode =std::pair<char, mode_list>;

act_mode parse_modes(const std::string& entries);


} //namespace irc

#endif //IRC_MODES_HPP