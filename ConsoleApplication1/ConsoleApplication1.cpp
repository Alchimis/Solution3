#include <iostream>
#include <optional>
#include <string>
#include <fstream>
#include <limits>
#include <vector>
#include <map>
#include <set>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graphviz.hpp>
#include "CUBTree.h"
#include <boost/graph/edge_connectivity.hpp>
#include <boost/graph/edge_list.hpp>

#include <boost/graph/adjacency_iterator.hpp>

using Matrix = std::vector<std::vector<int>>;

template <typename T>
std::optional<T> OpenFile(std::string name)
{
	T output;
	output.open(name);
	if (!output.good())
	{
		return std::nullopt;
	}
	return output;
};

void minimaze(const Matrix& matrix, std::map<std::vector<int>, int> groups, size_t index, std::map<std::vector<int>, int> nextGroup)
{

}

Matrix createMatrix(size_t size) {
	Matrix matrix;
	for (size_t i = 0;i < size;i++ )
	{
		std::vector<int> row;
		matrix.push_back(row);
	}
	return matrix;
}

Matrix readMatrixFromFile(std::ifstream& stream, int numberOfSignals,int numberOfStates) {
	Matrix matrix = createMatrix(numberOfStates);
	for (size_t i = 0; i < numberOfSignals; i++)
	{
		for (size_t k = 0; k < numberOfStates; k++)
		{ 
			size_t buff;
			stream >> buff;
			matrix[k].push_back(buff);
		}
	}
	stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	return matrix;
}

struct EdgeData
{
	std::string edge_name;
	double edge_confidence;
};

void someFunc(std::map<int, int>& groupMap, Matrix& matrixOfPerehody, std::map<int, int>& someOtherGroopNext)
{
	typedef boost::property<boost::edge_weight_t, int> EdgeWeightProperty;
	typedef boost::adjacency_list<boost::setS, boost::vecS, boost::directedS, boost::no_property, EdgeWeightProperty> Graph;
	typedef Graph::edge_descriptor Edge;
	Graph g;
	
	for (auto y : groupMap)
	{
		for (auto kkl : matrixOfPerehody[y.second - 1])
		{
			boost::add_edge_tag();
			std::pair<Edge, bool> ed =  boost::add_edge(someOtherGroopNext[y.second], someOtherGroopNext[kkl], g);
			
			int weight = get(boost::edge_weight_t(), g, ed.first);
			int weightToAdd = 10;
			boost::put(boost::edge_weight_t(), g, ed.first, 2);
			
		
		}
	}
	
	auto fileOut = OpenFile<std::ofstream>("Tt.dot");

	boost::write_graphviz(fileOut.value(), g);
}

int main()
{
	auto file = OpenFile<std::ifstream>("authoms.txt");//D:/cpluplus/repos/Solution3/ConsoleApplication1/authoms.txt
	int numOfStates, numOfInSignals, numOfOutSignals;
	file.value() >> numOfStates >> numOfInSignals >> numOfOutSignals;
	file.value().ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	Matrix matrixOfPerehody = readMatrixFromFile(file.value(),numOfInSignals,numOfStates);
	Matrix two = readMatrixFromFile(file.value(), numOfInSignals, numOfStates);
	std::map<std::vector<int>, int> groups;
	std::map<int, int> someOtherGroop;

	int y = 1;
	for (int i = 0; i < two.size();i++)
	{
		auto k = groups.find(two[i]);
		if (k == groups.end())
		{
			groups[two[i]] = y;
			y++;
		}
		someOtherGroop[i + 1] = groups[two[i]];
	}
	std::map<int, int> someOtherGroopPrev = someOtherGroop;
	std::map<int, int> someOtherGroopNext;
	//groups.clear();
	std::map<std::vector<int>, int> someOntherGroups;
	std::map<std::string, int> groupChanger;
	int prev = 0;
	int next = 1;
	while (prev != next)
	{
		someOtherGroopNext.clear();
		//groupMultimap.clear();
		int h = 1;
		for (size_t k = 0; k < matrixOfPerehody.size(); k++)
		{
			std::vector<int> vect;
			std::string vec = "";
			for (auto ll : matrixOfPerehody[k])
			{
				vec = vec + " " + std::to_string(someOtherGroopPrev[ll]);
			}
			auto findedGroup = groupChanger.find(vec);
			int groupNum;
			if (findedGroup == groupChanger.end())
			{
				groupChanger[vec] = h; 
				groupNum = h;
				h++;
			}
			else
			{
				groupNum = findedGroup->second;
			}
			someOtherGroopNext[k + 1] = groupNum; 
		}
		prev = next;
		next = groupChanger.size();
		someOtherGroopPrev = someOtherGroopNext;
		someOntherGroups.clear();
		groupChanger.clear();
	}

	std::map<int, int> groupMap;
	for (auto i : someOtherGroopNext)
	{
		auto j=groupMap.find(i.second);
		if (j == groupMap.end())
		{
			groupMap[i.second] = i.first;
		}
	}
	
	std::set<std::pair<int, int>> ajverts ;
		

	int size = 0;
	for (auto u : someOtherGroopNext)
	{
		if (size < u.second)
		{
			size = u.second;
		}
	}

	for (auto y : groupMap)
	{
		for (auto kkl : matrixOfPerehody[y.second-1])
		{
			ajverts.insert({ someOtherGroopNext[y.second], someOtherGroopNext[kkl] }); // buffGroopNext
		}
	}
	auto fileOut = OpenFile<std::ofstream>("Text.dot");

	boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS> bestGraph(ajverts.begin(),ajverts.end(),size );

	write_graphviz(fileOut.value(), bestGraph);
	write_graphviz(std::cout, bestGraph);
	someFunc(groupMap,matrixOfPerehody,someOtherGroopNext);
    return 0;
}












	/*CUBTree v (5);
std::vector<int>b{ 1, 2, 3, 4, 5};
std::vector<int> a{ 1, 2, 3, 5, 4 };
std::cout<<v.getSequenceGroup(b);
std::cout << v.getSequenceGroup(a);
std::cout << v.getSequenceGroup(b);
std::cout << v.getSequenceGroup(a);*/
