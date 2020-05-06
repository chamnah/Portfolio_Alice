#include "CInstBuffer.h"
#include "CDirect.h"
#include "CTexture.h"

CInstBuffer::CInstBuffer()
	:m_iMaxCount(10)
	, m_iAnimCount(0)
{
}

CInstBuffer::~CInstBuffer()
{
	SAFE_RELEASE(m_pInstBuff);
}

void CInstBuffer::Init()
{
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = sizeof(InstData) * m_iMaxCount;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if (FAILED(DEVICE->CreateBuffer(&desc, NULL, &m_pInstBuff)))
		assert(NULL);
}

void CInstBuffer::SetData(CTexture* _pBoneTex)
{
	if (m_vecInst.size() > m_iMaxCount)
	{
		Resize();
	}

	D3D11_MAPPED_SUBRESOURCE tMapData;
	CONTEXT->Map(m_pInstBuff, 0, D3D11_MAP_WRITE_DISCARD, 0, &tMapData);
	memcpy(tMapData.pData, &m_vecInst[0], sizeof(InstData) * m_vecInst.size());
	CONTEXT->Unmap(m_pInstBuff, 0);

	if (m_iAnimCount == 0 || _pBoneTex == nullptr)
		return;

	if (m_vecBone.size() * 4 > _pBoneTex->GetWidth() * _pBoneTex->GetHeight())
	{
		int iBoneCount = m_vecBone.size() / m_iAnimCount;
		_pBoneTex->Resize(iBoneCount*4, m_iAnimCount);
	}

	_pBoneTex->SetData(&m_vecBone[0],sizeof(Mtx) * m_vecBone.size());

}

void CInstBuffer::AddBoneMtx(vector<Mtx>& _mtxBone)
{
	m_iAnimCount++;
	m_vecBone.insert(m_vecBone.end(),_mtxBone.begin(),_mtxBone.end());
}

void CInstBuffer::Resize()
{
	m_iMaxCount = m_vecInst.size();

	SAFE_RELEASE(m_pInstBuff);

	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = sizeof(InstData) * m_iMaxCount;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if (FAILED(DEVICE->CreateBuffer(&desc, NULL, &m_pInstBuff)))
		assert(NULL);
}