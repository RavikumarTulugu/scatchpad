
#include<unistd.h>
#include<fcntl.h>
#include<iostream>
#include<set>
#include<boost/filesystem.hpp>

int main(int ac, char **av)
{
	std::set<std::string> execList;
	boost::filesystem::path _startAt("/proc");
	boost::system::error_code ec;
	boost::filesystem::recursive_directory_iterator _walker(_startAt, ec);
	while(_walker != boost::filesystem::recursive_directory_iterator()){
		if(ec){
			_walker.pop();
			if(_walker == boost::filesystem::recursive_directory_iterator()) 
				break;
			_walker.increment(ec);
			continue;
		}
		std::string leaf;
		std::string fullPath = _walker->path().string();
		size_t pos = fullPath.find_last_of("/");
		if(pos != std::string::npos){
			leaf.assign(fullPath.begin() + pos + 1, fullPath.end());
			if(leaf == "exe"){
				char buf[256];
				ssize_t rc = readlink(fullPath.c_str(), buf, sizeof(buf));
				if(rc > 0){
					std::string execName(buf, rc);
					execList.insert(execName);
				}
			}
		}
		_walker.increment(ec);
	}
	for(auto &s : execList) std::cerr<<"\nexec:"<<s;
	return 0;
}
