#ifndef _HEX_MESH_EDITOR_
#define _HEX_MESH_EDITOR_

#include "basehmesh.h"

namespace HMeshLib
{
	template<typename HM>
	class HexMeshEditor
	{
	public:
		HexMeshEditor(HM* inputMesh) {hmesh = inputMesh};
		~HexMeshEditor() {};

		typedef typename HM::CVertex V;
		typedef typename HM::CHVertex HV;
		typedef typename HM::CHalfEdge HalfE;
		typedef typename HM::CHEdge HE;
		typedef typename HM::CHalfFace HF;
		typedef typename HM::CFace F;
		typedef typename HM::CHex Hex;

		void changeVertex(Hex* hex, V* v,V* changeV);
		void separateFace(F* f);/*seprate the face to two faces*/
		V* creatVertex(CPoint p, int id);
	protected:
		HM* hmesh = NULL;
	};
	
	template<typename HM>
	typename HexMeshEditor<typename HM>::V* HexMeshEditor<typename HM>::creatVertex(CPoint p, int id)
	{
		V* v = new V();
		assert(v != NULL);
		v->position() = p;
		v->id() = id;
		hmesh->m_pVertices().push_back(v);
		hmesh->m_map_Vertices.insert(std::pair<int, CVertex*>(id, v));

	}

	template<typename HM>
	void HexMeshEditor<typename HM>:: changeVertex(Hex* hex, V* v,V* changeV)
	{
		/*
		* ��Ҫ�޸ĵ����ݣ�
		*	����һ���µĵ�
		*	�ҵ����оɵĵ������hex
		*		Hex���޸��������й�����ߵĵ�
		*		�޸�vertex�����е�Hvertices�� edges
		*		�޸����еĹ����ıߵ�ֵ
		*		
		*		�޸����й�����HVertex
		*	ɾ���ɵ�	
		*/
		//construct a new vertex

		//���ֻ�޸�λ�ã����޸����ӹ�ϵ�Ļ���ֻ�޸����꼴��
	}
	template<typename HM>
	void HexMeshEditor<typename HM>::separateFace(F* f)
	{
		/**��һ�������Ϊ������*/
		/*
		* ��һ�������Ϊ�����棬������һ��������
		*	�������ϵ��ĸ��㣬�����ϵ���������߹�ϵ���ֱ�ֵ�����ߡ�
		*	
		*/
		
	}
}

#endif // !_HEX_MESH_EDITOR_
