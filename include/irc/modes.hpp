#ifndef IRC_MODES_HPP
#define IRC_MODES_HPP

#include "types.hpp"

#include <ostream>
#include <vector>

namespace irc {

enum class mode_change : bool {
	set, unset
}; //enum class mode_change

struct mode_diff {
	mode_change change;
	mode_list   modes;
}; //struct mode_diff

class mode_block {
public:
	using value_type    =mode_entry;
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
	void apply_mode_diff(const prefix& p, const mode_diff& md);

	optional_string try_get_mode_param(char sym);

	template<typename F> bsig::connection connect_on_mode_change(F&& f);
private:
	void set_mode_impl(char sym, const optional_string& param);
	void unset_mode_impl(char sym);

	sig_p_md  on_mode_change;
	mode_list modes;
}; //class modes_block

template<typename F>
bsig::connection mode_block::connect_on_mode_change(F&& f) {
	return on_mode_change.connect(std::forward<F>(f));
}

std::ostream& operator<<(std::ostream& os, const mode_block& mb);
std::ostream& operator<<(std::ostream& os, const mode_list& ml);
std::ostream& operator<<(std::ostream& os, const mode_diff& md);

std::string to_string(const mode_block& mb);
std::string to_string(const mode_list& ml);
std::string to_string(const mode_diff& md);

mode_diff parse_modes(const std::string& entries);

} //namespace irc

#endif //IRC_MODES_HPP
