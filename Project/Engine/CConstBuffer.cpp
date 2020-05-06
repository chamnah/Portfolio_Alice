#include "CConstBuffer.h"
#include "CDirect.h"


CConstBuffer* CConstBuffer::Create(UINT _iSize,UINT _iRegister)
{
	D3D11_BUFFER_DESC tBufferDesc = {};
	tBufferDesc.ByteWidth = _iSize;
	tBufferDesc.Usage = D3D11_USAGE_DYNAMIC; // cpu에서 가변적으로 gpu에 접근해서 변경할때 쓰인다.
	tBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // cpu가 접근 안하려면 0 , 접근해서 갱신하려면 write
	tBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // 이녀석을 어떤 녀석인지 표기 여기선 VertexBuffer라고 알림
	tBufferDesc.MiscFlags = 0; // 정점버퍼에서는 굳이 지정할 필요는 없다.
	tBufferDesc.StructureByteStride = 0; // 구조적 버퍼에 저장된 원소 하나의 크기 구조적버퍼를 위해서만 필요 그외에는 0

	ID3D11Buffer* pCB = nullptr;

	DEVICE->CreateBuffer(&tBufferDesc, NULL, &pCB);

	CConstBuffer* pBuffer = new CConstBuffer;
	pBuffer->m_pCB = pCB;
	pBuffer->m_iRegister = _iRegister;

	return pBuffer;
}

void CConstBuffer::UpdateData(void* _pData,UINT _iSize)
{
	D3D11_MAPPED_SUBRESOURCE tSub = {};
	CONTEXT->Map(m_pCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	memcpy(tSub.pData, _pData, _iSize);
	CONTEXT->Unmap(m_pCB, 0);
}

void CConstBuffer::ClearData()
{
	ID3D11Buffer* pBuffer = nullptr;

	CONTEXT->VSSetConstantBuffers(m_iRegister, 1, &pBuffer);
	CONTEXT->HSSetConstantBuffers(m_iRegister, 1, &pBuffer);
	CONTEXT->DSSetConstantBuffers(m_iRegister, 1, &pBuffer);
	CONTEXT->GSSetConstantBuffers(m_iRegister, 1, &pBuffer);
	CONTEXT->CSSetConstantBuffers(m_iRegister, 1, &pBuffer);
	CONTEXT->PSSetConstantBuffers(m_iRegister, 1, &pBuffer);
}

void CConstBuffer::UpdateRegister()
{
	CONTEXT->VSSetConstantBuffers(m_iRegister, 1, &m_pCB);
	CONTEXT->HSSetConstantBuffers(m_iRegister, 1, &m_pCB);
	CONTEXT->DSSetConstantBuffers(m_iRegister, 1, &m_pCB);
	CONTEXT->GSSetConstantBuffers(m_iRegister, 1, &m_pCB);
	CONTEXT->CSSetConstantBuffers(m_iRegister, 1, &m_pCB);
	CONTEXT->PSSetConstantBuffers(m_iRegister, 1, &m_pCB);
}

void CConstBuffer::UpdateRegisterAll()
{
	CONTEXT->VSSetConstantBuffers(m_iRegister, 1, &m_pCB);
	CONTEXT->HSSetConstantBuffers(m_iRegister, 1, &m_pCB);
	CONTEXT->DSSetConstantBuffers(m_iRegister, 1, &m_pCB);
	CONTEXT->GSSetConstantBuffers(m_iRegister, 1, &m_pCB);
	CONTEXT->CSSetConstantBuffers(m_iRegister, 1, &m_pCB);
	CONTEXT->PSSetConstantBuffers(m_iRegister, 1, &m_pCB);
}

CConstBuffer::CConstBuffer()
{
}

CConstBuffer::~CConstBuffer()
{
	SAFE_RELEASE(m_pCB);
}