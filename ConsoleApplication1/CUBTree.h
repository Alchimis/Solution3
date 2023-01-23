#pragma once
#include <map>
#include <memory>
#include <vector>
class CUBTree
{


private:

	int m_treeSize;
	class Node
	{
	public:
		virtual std::shared_ptr<Node> getPtr(int i, int treeSize) = 0;
		virtual std::shared_ptr<Node> addNode(int i, std::shared_ptr<Node>) = 0;
		virtual int getGroup() =0;
	};
	class ChainNode : public Node
	{
		const int m_depth;
	private:
		std::map<int, std::shared_ptr<Node>> m_ptrMap;
		
	public:
		int getGroup() override { return 0; };
		std::shared_ptr<Node> addNode(int i, std::shared_ptr<Node> node) override
		{
			m_ptrMap[i] = node;
			//m_ptrMap.insert({i,node});
			return node;
		};
		ChainNode(int depth)
			: m_depth(depth)
		{
		}
		std::shared_ptr<Node> getPtr(int i, int treeSize) override{
			auto finded = m_ptrMap.find(i);
			if (finded != m_ptrMap.end())
			{
				return finded->second;
			}
			//if (m_depth < treeSize)
			//{
			//	auto ptr = std::make_shared<ChainNode>(ChainNode(m_depth+1));
			//	m_ptrMap[i] = ptr;
			//	return ptr;
			//}
			//auto ptr = std::make_shared<EndNode>(EndNode());
			//m_ptrMap[i] = ptr;
			return nullptr;
			
		};
	};
	class EndNode : public Node
	{
		const int m_i;
		
	public:
		int getGroup() override { return m_i; };
		std::shared_ptr<Node> addNode(int i, std::shared_ptr<Node>) override { return nullptr; };
		std::shared_ptr<Node> getPtr(int i, int treeSize) override { return std::shared_ptr<EndNode>(this); };
		EndNode(int i)
			: m_i(i)
		{
		}
	};
	std::map<int, std::shared_ptr<Node>> m_ptrMap;
	int groupsCount;

public:
	CUBTree(int treeSize)
		: m_treeSize(treeSize)
		, groupsCount(0)
		
	{
	
	}
	

	int getSequenceGroup(std::vector<int>& sequence) 
	{
		auto fastener = sequence.begin();
		auto finded = m_ptrMap.find(*fastener);
		if (finded == m_ptrMap.end())
		{
			
			m_ptrMap.insert({ *fastener, std::make_shared<ChainNode>(1) });
			finded = m_ptrMap.find(*fastener);
		}
		fastener++;
		auto pr = finded->second->getPtr(*fastener, m_treeSize);
		int depth = 0;
		if (pr == nullptr)
		{
			pr = finded->second->addNode(*fastener, std::make_shared<ChainNode>(depth++));
		}
		
		for (;fastener != sequence.end();fastener++)
		{
			auto kp = pr->getPtr(*fastener, m_treeSize);
			if (kp == nullptr)
			{
				kp = pr->addNode(*fastener, std::make_shared<ChainNode>(depth++));
			}
			pr = kp;

		}
		auto kp = pr->getPtr(0, m_treeSize);
		if (kp == nullptr)
		{
			kp = pr->addNode(0, std::make_shared<EndNode>(EndNode(groupsCount)));
			groupsCount++;		
		}
		return kp->getGroup();
	}
};
