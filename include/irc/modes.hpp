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
	using mode_list     =std::vector<value_type>;

	void insert(char sym);
	void insert(char sym, const std::string& param);
	void erase(char sym);

	const_iterator begin() const;
	const_iterator end() const;

	iterator       find(char sym);
	const_iterator find(char sym) const;

	bool is_mode_set(char sym) const;

	void set_mode(const prefix& p, char sym, const optional_string& param);
	void set_mode(const prefix& p, const mode_list& modes);
	void unset_mode(const prefix& p, char sym);
	void unset_mode(const prefix& p, const mode_list& modes);
	optional_string try_get_mode_param(char sym);

	template<typename F> bsig::connection connect_on_set_mode(F&& f);
private:
	void set_mode_impl(char sym, const optional_string& param);
	void unset_mode_impl(char sym);


	sig_p_vcos  on_unset_mode;
	sig_p_vcos  on_set_mode;
	mode_list   modes;
}; //class modes_block

template<typename F>
bsig::connection mode_block::connect_on_set_mode(F&& f) {
	return on_set_mode.connect(std::forward<F>(f));
}

std::ostream& operator<<(std::ostream& os, const mode_block& mb);



using mode_list=std::vector<mode_block::value_type>;
using act_mode =std::pair<char, mode_list>;

std::string to_string(const mode_block& mb);
std::string to_string(const mode_list& ml);

act_mode parse_modes(const std::string& entries);

} //namespace irc

#endif //IRC_MODES_HPP
