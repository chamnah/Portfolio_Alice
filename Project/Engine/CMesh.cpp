#include "CMesh.h"
#include "CDirect.h"
#include "CShader.h"
#include "CMaterial.h"
#include "CFBXLoader.h"
#include "CInstBuffer.h"
#include "CResMgr.h"

CMesh * CMesh::CreateMesh(void * _pVTX, int _iVTXSize, int _iVTXCount, void * _pITX, int _iITXSize, int _iITXCount)
{
	CMesh* pMesh = new CMesh;

	D3D11_BUFFER_DESC tBufferDesc = {};
	tBufferDesc.ByteWidth = _iVTXSize * _iVTXCount;
	tBufferDesc.Usage = D3D11_USAGE_DEFAULT; // cpu���� ���������� gpu�� �����ؼ� �����Ҷ� ���δ�.
	tBufferDesc.CPUAccessFlags = 0; // cpu�� ���� ���Ϸ��� 0 , �����ؼ� �����Ϸ��� write
	tBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // �̳༮�� � �༮���� ǥ�� ���⼱ VertexBuffer��� �˸�
	tBufferDesc.MiscFlags = 0; // �������ۿ����� ���� ������ �ʿ�� ����.
	tBufferDesc.StructureByteStride = 0; // ������ ���ۿ� ����� ���� �ϳ��� ũ�� ���������۸� ���ؼ��� �ʿ� �׿ܿ��� 0

	D3D11_SUBRESOURCE_DATA tSubData = {};
	tSubData.pSysMem = _pVTX;

	ID3D11Buffer* pVB = nullptr;
	DEVICE->CreateBuffer(&tBufferDesc, &tSubData, &pVB);

	pMesh->m_descVB = tBufferDesc;

	ID3D11Buffer* pDrawVB = nullptr;
	ID3D11Buffer* pStreamVB = nullptr;

	tBufferDesc = {};
	tBufferDesc.ByteWidth = _iITXSize * _iITXCount;
	tBufferDesc.Usage = D3D11_USAGE_DEFAULT; // cpu���� ���������� gpu�� �����ؼ� �����Ҷ� ���δ�.
	tBufferDesc.CPUAccessFlags = 0; // cpu�� ���� ���Ϸ��� 0 , �����ؼ� �����Ϸ��� write
	tBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER; // �̳༮�� � �༮���� ǥ�� ���⼱ VertexBuffer��� �˸�
	tBufferDesc.MiscFlags = 0; // �������ۿ����� ���� ������ �ʿ�� ����.
	tBufferDesc.StructureByteStride = 0; // ������ ���ۿ� ����� ���� �ϳ��� ũ�� ���������۸� ���ؼ��� �ʿ� �׿ܿ��� 0

	tSubData = {};
	tSubData.pSysMem = _pITX;

	tIndexInfo idxInfo = {};
	idxInfo.iIdxSize = _iITXSize;
	idxInfo.iIdxCount = _iITXCount;
	idxInfo.tDesc = tBufferDesc;

	ID3D11Buffer* pIB = nullptr;
	DEVICE->CreateBuffer(&tBufferDesc, &tSubData, &idxInfo.pIB);

	// Input�� �ڷ��� ����ü�� ���� ������ ��´�.
	// directx ������� pragma pack(1) �� ����Ǿ ����ü�� �� ũ�� �״���̴�.

	pMesh->m_pVB = pVB;
	pMesh->m_iVtxSize = _iVTXSize;
	pMesh->m_iVtxCount = _iVTXCount;
	//pMesh->m_pVtxMem = _pVTX;
	pMesh->m_vecIdxInfo.push_back(idxInfo);
	
	pMesh->AddInputLayout("POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT,0,0);
	pMesh->AddInputLayout("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
	pMesh->AddInputLayout("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0);
	pMesh->AddInputLayout("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0);
	pMesh->AddInputLayout("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0);
	pMesh->AddInputLayout("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0);
	pMesh->AddInputLayout("WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
	pMesh->AddInputLayout("INDICIES", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
	pMesh->AddInputLayout("AGE", 0, DXGI_FORMAT_R32_FLOAT, 0, 0);
	pMesh->AddInputLayout("TYPE", 0, DXGI_FORMAT_R32_UINT, 0, 0);

	pMesh->AddInputLayout("WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	pMesh->AddInputLayout("WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	pMesh->AddInputLayout("WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	pMesh->AddInputLayout("WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);

	pMesh->AddInputLayout("WV", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	pMesh->AddInputLayout("WV", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	pMesh->AddInputLayout("WV", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	pMesh->AddInputLayout("WV", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);

	pMesh->AddInputLayout("VP", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	pMesh->AddInputLayout("VP", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	pMesh->AddInputLayout("VP", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	pMesh->AddInputLayout("VP", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);

	pMesh->AddInputLayout("WVP", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	pMesh->AddInputLayout("WVP", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	pMesh->AddInputLayout("WVP", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	pMesh->AddInputLayout("WVP", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);

	pMesh->AddInputLayout("ROWINDEX", 0, DXGI_FORMAT_R32_SINT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	pMesh->AddInputLayout("UV", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	return pMesh;
}

CMesh * CMesh::CreateMesh(void * _pVTX, int _iVTXSize, int _iVTXCount, vector<tIndexInfo>& _vecInfo)
{
	CMesh* pMesh = new CMesh;

	D3D11_BUFFER_DESC tBufferDesc = {};
	tBufferDesc.ByteWidth = _iVTXSize * _iVTXCount;
	tBufferDesc.Usage = D3D11_USAGE_DEFAULT; // cpu���� ���������� gpu�� �����ؼ� �����Ҷ� ���δ�.
	tBufferDesc.CPUAccessFlags = 0; // cpu�� ���� ���Ϸ��� 0 , �����ؼ� �����Ϸ��� write
	tBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // �̳༮�� � �༮���� ǥ�� ���⼱ VertexBuffer��� �˸�
	tBufferDesc.MiscFlags = 0; // �������ۿ����� ���� ������ �ʿ�� ����.
	tBufferDesc.StructureByteStride = 0; // ������ ���ۿ� ����� ���� �ϳ��� ũ�� ���������۸� ���ؼ��� �ʿ� �׿ܿ��� 0

	D3D11_SUBRESOURCE_DATA tSubData = {};
	tSubData.pSysMem = _pVTX;

	ID3D11Buffer* pVB = nullptr;
	DEVICE->CreateBuffer(&tBufferDesc, &tSubData, &pVB);

	for (size_t i = 0; i < _vecInfo.size(); i++)
	{
		tIndexInfo Info = _vecInfo[i];
		tSubData.pSysMem = Info.pSysMem;

		DEVICE->CreateBuffer(&Info.tDesc, &tSubData, &Info.pIB);
		pMesh->m_vecIdxInfo.push_back(Info);
	}

	pMesh->m_descVB = tBufferDesc;
	pMesh->m_pVB = pVB;
	pMesh->m_iVtxSize = _iVTXSize;
	pMesh->m_iVtxCount = _iVTXCount;
	pMesh->m_pVtxMem = _pVTX;

	ID3D11Buffer* pDrawVB = nullptr;
	ID3D11Buffer* pStreamVB = nullptr;

	// Input�� �ڷ��� ����ü�� ���� ������ ��´�.
	// directx ������� pragma pack(1) �� ����Ǿ ����ü�� �� ũ�� �״���̴�.

	pMesh->AddInputLayout("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0);
	pMesh->AddInputLayout("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
	pMesh->AddInputLayout("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0);
	pMesh->AddInputLayout("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0);
	pMesh->AddInputLayout("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0);
	pMesh->AddInputLayout("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0);
	pMesh->AddInputLayout("WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
	pMesh->AddInputLayout("INDICIES", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
	pMesh->AddInputLayout("AGE", 0, DXGI_FORMAT_R32_FLOAT, 0, 0);
	pMesh->AddInputLayout("TYPE", 0, DXGI_FORMAT_R32_UINT, 0, 0);

	pMesh->AddInputLayout("WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	pMesh->AddInputLayout("WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	pMesh->AddInputLayout("WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	pMesh->AddInputLayout("WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);

	pMesh->AddInputLayout("WV", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	pMesh->AddInputLayout("WV", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	pMesh->AddInputLayout("WV", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	pMesh->AddInputLayout("WV", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);

	pMesh->AddInputLayout("VP", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	pMesh->AddInputLayout("VP", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	pMesh->AddInputLayout("VP", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	pMesh->AddInputLayout("VP", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);

	pMesh->AddInputLayout("WVP", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	pMesh->AddInputLayout("WVP", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	pMesh->AddInputLayout("WVP", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	pMesh->AddInputLayout("WVP", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);

	pMesh->AddInputLayout("ROWINDEX", 0, DXGI_FORMAT_R32_SINT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	pMesh->AddInputLayout("UV", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	return pMesh;
}

void CMesh::CreateStreamVB(UINT _iCount)
{
	m_bStreamVB = true;

	if (m_pDrawVB != nullptr || m_pStreamVB != nullptr)
	{
		SAFE_RELEASE(m_pDrawVB);
		SAFE_RELEASE(m_pStreamVB);
	}
	D3D11_BUFFER_DESC desc = {};
	m_pVB->GetDesc(&desc);

	desc.ByteWidth = m_iVtxSize * _iCount;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;

	DEVICE->CreateBuffer(&desc,0,&m_pDrawVB);
	DEVICE->CreateBuffer(&desc, 0, &m_pStreamVB);
}

void CMesh::SetShader(CShader* _pShader,bool _bInstance)
{
	ID3DBlob* pBlob = nullptr;
	if (_bInstance)
		pBlob = _pShader->GetVSInstBlob();
	else
		pBlob = _pShader->GetVSBlob();

	if (m_pVSBlob == pBlob)
		return;

	m_pVSBlob = pBlob;
	SAFE_RELEASE(m_pLayout);
	DEVICE->CreateInputLayout(&m_vecLayout[0], (int)m_vecLayout.size(), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &m_pLayout);
}

void CMesh::Render(int _iSubSet)
{
	if (m_vecIdxInfo[_iSubSet].iIdxCount == 0)
		return;

	CONTEXT->IASetInputLayout(m_pLayout);

	UINT iOffset = 0; // ���° �������� ��������
	UINT iStride = sizeof(VTX); // �� ������ ũ��, ���� �������� ����

	CONTEXT->IASetVertexBuffers(0, // ���� ���۵��� ���̱� ������ �Է½����� ���� Layout�������� ������ �Է½����� ���ϴ°� ����.
		1, // �Է� ���Կ��ٰ� ���� ���� ����
		&m_pVB, &iStride, &iOffset);

	CONTEXT->IASetIndexBuffer(m_vecIdxInfo[_iSubSet].pIB, DXGI_FORMAT_R32_UINT, 0);
	CONTEXT->DrawIndexed(m_vecIdxInfo[_iSubSet].iIdxCount * (m_vecIdxInfo[_iSubSet].iIdxSize / 4), // ���� �׸� �ε��� ����
		0, // �׸��� ������ ù �ε���
		0); // �׸��� ������ ù ����
}

void CMesh::InstancingRender(int _iSubSet)
{
	CONTEXT->IASetInputLayout(m_pLayout);

	ID3D11Buffer* arrBuffer[2] = { m_pVB ,CInstBuffer::GetInst()->GetInstBuffer() };
	UINT iOffset[2] = {0,0}; // ���° �������� ��������
	UINT iStride[2] = { sizeof(VTX),sizeof(InstData) }; // �� ������ ũ��, ���� �������� ����

	CONTEXT->IASetVertexBuffers(0, // ���� ���۵��� ���̱� ������ �Է½����� ���� Layout�������� ������ �Է½����� ���ϴ°� ����.
		2, // �Է� ���Կ��ٰ� ���� ���� ���� 
		arrBuffer, iStride, iOffset);

	CONTEXT->IASetIndexBuffer(m_vecIdxInfo[_iSubSet].pIB, DXGI_FORMAT_R32_UINT, 0);
	UINT iCount = CInstBuffer::GetInst()->GetInstBufferCount();
	CONTEXT->DrawIndexedInstanced(m_vecIdxInfo[_iSubSet].iIdxCount * (m_vecIdxInfo[_iSubSet].iIdxSize / 4), iCount,0,0,0);
}

//void CMesh::Render()
//{
//	CONTEXT->IASetInputLayout(m_pLayout);
//
//	UINT iOffset = 0; // ���° �������� ��������
//	UINT iStride = sizeof(VTX); // �� ������ ũ��, ���� �������� ����
//
//	CONTEXT->IASetVertexBuffers(0, // ���� ���۵��� ���̱� ������ �Է½����� ���� Layout�������� ������ �Է½����� ���ϴ°� ����.
//		1, // �Է� ���Կ��ٰ� ���� ���� ���� 
//		&m_pVB, &iStride, &iOffset);
//
//	CONTEXT->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R32_UINT, 0);
//	CONTEXT->DrawIndexed(m_iIdxCount * (m_iIdxSize / 4), // ���� �׸� �ε��� ����
//		0, // �׸��� ������ ù �ε���
//		0); // �׸��� ������ ù ����
//}

void CMesh::StreamRender(CMaterial * _StreamMtrl, CMaterial * _DrawMtrl)
{
	_StreamMtrl->UpdateData();
	SetShader(_StreamMtrl->GetShader());

	CONTEXT->IASetInputLayout(m_pLayout);

	UINT iOffset = 0; // ���° �������� ��������
	UINT iStride = sizeof(VTX); // �� ������ ũ��, ���� �������� ����

	CONTEXT->SOSetTargets(1, &m_pStreamVB, &iOffset);
	if (m_bFirstParticle)
	{
		m_bFirstParticle = false;
		CONTEXT->IASetVertexBuffers(0, 1, &m_pVB, &iStride, &iOffset);
		CONTEXT->Draw(1, 0);
	}
	else
	{
		CONTEXT->IASetVertexBuffers(0, 1, &m_pDrawVB, &iStride, &iOffset);
		CONTEXT->DrawAuto();
	}

	ID3D11Buffer* buffers[1] = { 0 };
	CONTEXT->SOSetTargets(1, buffers, &iOffset);

	std::swap(m_pStreamVB, m_pDrawVB);

	_DrawMtrl->UpdateData();
	SetShader(_DrawMtrl->GetShader());

	CONTEXT->IASetVertexBuffers(0, 1, &m_pDrawVB, &iStride, &iOffset);
	CONTEXT->DrawAuto();
}

void CMesh::VertexRender(int _iSubSet,int _iSize)
{
	if (m_vecIdxInfo[_iSubSet].iIdxCount == 0)
		return;

	CONTEXT->IASetInputLayout(m_pLayout);

	UINT iOffset = 0; // ���° �������� ��������
	UINT iStride = sizeof(VTX); // �� ������ ũ��, ���� �������� ����

	CONTEXT->IASetVertexBuffers(0, // ���� ���۵��� ���̱� ������ �Է½����� ���� Layout�������� ������ �Է½����� ���ϴ°� ����.
		1, // �Է� ���Կ��ٰ� ���� ���� ����
		&m_pVB, &iStride, &iOffset);

	if (_iSize > m_iVtxCount)
		_iSize = m_iVtxCount;
	CONTEXT->Draw(_iSize,0);
}

// ���⼭ FBX Container���� ������� ��Ƽ� �����Ѱ��� ���⼭ ����ü ���·� ��ȯ
CMesh* CMesh::CreateFromContainer(CFBXLoader * _loader)
{
	vector<tContainer>& vecContainer = _loader->GetContainer();
	vector<tBone*>& vecBone = _loader->GetBone();
	vector<tAnimClip*>& vecAnimClip = _loader->GetAnimClip();

	CMesh* pMesh = new CMesh;

	for (size_t i = 0; i < vecContainer.size(); i++)
	{
		UINT iSize = vecContainer[i].vecVtxPos.size();
		D3D11_BUFFER_DESC desc;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(VTX) * iSize;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		desc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA SubData = {};
		SubData.pSysMem = /*new VTX[desc.ByteWidth];*/malloc(desc.ByteWidth);
		VTX* pData = (VTX*)SubData.pSysMem;

		for (size_t j = 0; j < iSize; j++)
		{
			pData[j] = {};
			pData[j].vPos = vecContainer[i].vecVtxPos[j];
			pData[j].vNormal = vecContainer[i].vecVtxNormal[j];
			pData[j].vBinormal = vecContainer[i].vecVtxBinormal[j];
			pData[j].vTangent = vecContainer[i].vecVtxTangent[j];
			pData[j].vTex = vecContainer[i].vecVtxUV[j];

			pData[j].vWeight = vecContainer[i].vecWeight[j];
			pData[j].vIndices = vecContainer[i].vecIndices[j];
		}

		ID3D11Buffer* pVB = nullptr;
		DEVICE->CreateBuffer(&desc,&SubData, &pVB);
		pMesh->m_pVB = pVB;
		pMesh->m_iVtxCount = iSize;
		pMesh->m_iVtxSize = sizeof(VTX);
		pMesh->m_pVtxMem = pData;

		for (size_t j = 0; j < vecContainer[i].vecIdx.size(); j++) // �ϳ��� �����̳ʿ� ����� ������ŭ ������.
		{
			desc = {};
			SubData = {};
			tIndexInfo Info = {};

			Info.iIdxCount = vecContainer[i].vecIdx[j].size();

			if (Info.iIdxCount == 0)
			{
				pMesh->m_vecIdxInfo.push_back(Info);
				continue;
			}
			Info.iIdxSize = sizeof(UINT);

			desc.ByteWidth = Info.iIdxSize * Info.iIdxCount;
			desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;
			desc.StructureByteStride = 0;
			desc.Usage = D3D11_USAGE_DEFAULT;

			Info.pSysMem = malloc(desc.ByteWidth);
			memcpy(Info.pSysMem, &vecContainer[i].vecIdx[j][0], desc.ByteWidth);
			SubData.pSysMem = Info.pSysMem;

			DEVICE->CreateBuffer(&desc, &SubData, &Info.pIB);

			Info.tDesc = desc;
			pMesh->m_vecIdxInfo.push_back(Info);
		}

		if (!vecContainer[i].bAnimaion)
			continue;

		for (size_t j = 0; j < vecBone.size(); j++)
		{
			tMtxBone tBone = {};
			tBone.strName = vecBone[j]->strName;
			tBone.iDepth = vecBone[j]->iDepth;
			tBone.iParent = vecBone[j]->iParent;
			tBone.OffsetMtx = GetMatrix(vecBone[j]->mtxOffset);
			tBone.TransForm = GetMatrix(vecBone[j]->mtxTransForm);

			for (size_t k = 0; k < vecBone[j]->vecKeyFrame.size(); k++)
			{
				tMtxKeyFrm Frm = {};

				Frm.fTime = vecBone[j]->vecKeyFrame[k].dTime;
				Frm.iFrm = k;

				Frm.vTranslate.x = vecBone[j]->vecKeyFrame[k].matTransform.GetT().mData[0];
				Frm.vTranslate.y = vecBone[j]->vecKeyFrame[k].matTransform.GetT().mData[1];
				Frm.vTranslate.z = vecBone[j]->vecKeyFrame[k].matTransform.GetT().mData[2];

				Frm.vScale.x = vecBone[j]->vecKeyFrame[k].matTransform.GetS().mData[0];
				Frm.vScale.y = vecBone[j]->vecKeyFrame[k].matTransform.GetS().mData[1];
				Frm.vScale.z = vecBone[j]->vecKeyFrame[k].matTransform.GetS().mData[2];

				Frm.vQRot.x = vecBone[j]->vecKeyFrame[k].matTransform.GetQ().mData[0];
				Frm.vQRot.y = vecBone[j]->vecKeyFrame[k].matTransform.GetQ().mData[1];
				Frm.vQRot.z = vecBone[j]->vecKeyFrame[k].matTransform.GetQ().mData[2];
				Frm.vQRot.w = vecBone[j]->vecKeyFrame[k].matTransform.GetQ().mData[3];

				tBone.vecKeyFrm.push_back(Frm);
			}

			pMesh->m_vecBone.push_back(tBone);
		}


		for (size_t j = 0; j < vecAnimClip.size(); j++)
		{
			tMtxAnimClip AnimClip = {};
			AnimClip.strName = vecAnimClip[j]->strName;
			AnimClip.dStartTime = vecAnimClip[j]->tStartTime.GetSecondDouble();
			AnimClip.dEndTime = vecAnimClip[j]->tEndTime.GetSecondDouble();
			AnimClip.dTimeLength = AnimClip.dEndTime - AnimClip.dStartTime;
			AnimClip.iStartFrm = vecAnimClip[j]->tStartTime.GetFrameCount(vecAnimClip[j]->eMode);
			AnimClip.iEndFrm = vecAnimClip[j]->tEndTime.GetFrameCount(vecAnimClip[j]->eMode);
			AnimClip.iFrmLength = AnimClip.iEndFrm - AnimClip.iStartFrm;
			AnimClip.eMode = vecAnimClip[j]->eMode;

			pMesh->m_vecAnimClip.push_back(AnimClip);
		}
	}

	pMesh->AddInputLayout("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0);
	pMesh->AddInputLayout("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
	pMesh->AddInputLayout("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0);
	pMesh->AddInputLayout("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0);
	pMesh->AddInputLayout("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0);
	pMesh->AddInputLayout("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0);
	pMesh->AddInputLayout("WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
	pMesh->AddInputLayout("INDICIES", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
	pMesh->AddInputLayout("AGE", 0, DXGI_FORMAT_R32_FLOAT, 0, 0);
	pMesh->AddInputLayout("TYPE", 0, DXGI_FORMAT_R32_UINT, 0, 0);
	
	pMesh->AddInputLayout("WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	pMesh->AddInputLayout("WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	pMesh->AddInputLayout("WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	pMesh->AddInputLayout("WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);

	pMesh->AddInputLayout("WV", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	pMesh->AddInputLayout("WV", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	pMesh->AddInputLayout("WV", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	pMesh->AddInputLayout("WV", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);

	pMesh->AddInputLayout("VP", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	pMesh->AddInputLayout("VP", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	pMesh->AddInputLayout("VP", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	pMesh->AddInputLayout("VP", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);

	pMesh->AddInputLayout("WVP", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	pMesh->AddInputLayout("WVP", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	pMesh->AddInputLayout("WVP", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	pMesh->AddInputLayout("WVP", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);

	pMesh->AddInputLayout("ROWINDEX", 0, DXGI_FORMAT_R32_SINT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	pMesh->AddInputLayout("UV", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	return pMesh;
}

void CMesh::AddInputLayout(const char * _pSemanticName, UINT _iSemanticIdx, DXGI_FORMAT _eFormat, UINT _iInputSlot, UINT _iInstancing, D3D11_INPUT_CLASSIFICATION _eInputClass)
{
	D3D11_INPUT_ELEMENT_DESC tLayoutDesc;

	tLayoutDesc.SemanticName = _pSemanticName;
	tLayoutDesc.SemanticIndex = _iSemanticIdx; // ���� �ø�ƽ�� ���� �ε��� POSITION 0 ~ ....
	tLayoutDesc.Format = _eFormat;
	tLayoutDesc.InputSlot = _iInputSlot; //�� 16�� 0 ~ 15�� ���� ������ �غ�Ǿ� ������ �ϳ��� �Է½������� ���������ο� �����Ҽ��� �ְ� ����� �����Ҽ��� �ִ�.
	tLayoutDesc.InputSlotClass = _eInputClass; // �ν��Ͻ̿� ���̴� ��� �ϴ� �̷��� ������ �صΰ� ������ ����
	tLayoutDesc.InstanceDataStepRate = _iInstancing; // �ν��Ͻ̿� ���̴� ��� �ϴ� �̷��� ������ �صΰ� ������ ����

	if (_iInputSlot == 0)
	{
		tLayoutDesc.AlignedByteOffset = m_iOffet; // �� ������ ����ü���� ����Ʈ���� ��ġ�� �������
		m_iOffet += GetSizeofFormat(_eFormat);
	}
	else
	{
		tLayoutDesc.AlignedByteOffset = m_iOffetInst;
		m_iOffetInst += GetSizeofFormat(_eFormat);
	}

	m_vecLayout.push_back(tLayoutDesc);
}

void CMesh::SaveMeshData(FILE * _pFile)
{
	SaveWString(GetName(),_pFile);
	fwrite(&m_iVtxSize, sizeof(int), 1, _pFile);
	fwrite(&m_iVtxCount, sizeof(int), 1, _pFile);

	int iByteSize = m_iVtxSize * m_iVtxCount;
	fwrite(&iByteSize, sizeof(int), 1, _pFile);
	fwrite(m_pVtxMem, iByteSize, 1, _pFile);
	fwrite(&m_descVB, sizeof(D3D11_BUFFER_DESC), 1, _pFile);

	UINT iMtrlCount = m_vecIdxInfo.size();
	fwrite(&iMtrlCount, sizeof(UINT), 1, _pFile);

	for (size_t i = 0; i < iMtrlCount; i++)
	{
		fwrite(&m_vecIdxInfo[i],sizeof(tIndexInfo),1,_pFile);
		fwrite(m_vecIdxInfo[i].pSysMem, m_vecIdxInfo[i].iIdxSize * m_vecIdxInfo[i].iIdxCount, 1, _pFile);
	}

	UINT iAnimSize = m_vecAnimClip.size();
	fwrite(&iAnimSize, sizeof(UINT), 1, _pFile);

	for (size_t i = 0; i < iAnimSize; i++)
	{
		SaveWString(m_vecAnimClip[i].strName,_pFile);
		fwrite(&m_vecAnimClip[i].dStartTime,sizeof(double),1,_pFile);
		fwrite(&m_vecAnimClip[i].dEndTime, sizeof(double), 1, _pFile);
		fwrite(&m_vecAnimClip[i].dTimeLength, sizeof(double), 1, _pFile);
		fwrite(&m_vecAnimClip[i].iStartFrm, sizeof(int), 1, _pFile);
		fwrite(&m_vecAnimClip[i].iEndFrm, sizeof(int), 1, _pFile);
		fwrite(&m_vecAnimClip[i].iFrmLength, sizeof(int), 1, _pFile);
		fwrite(&m_vecAnimClip[i].eMode, sizeof(int), 1, _pFile);
	}

	UINT iBoneSize = m_vecBone.size();
	fwrite(&iBoneSize, sizeof(UINT), 1, _pFile);

	for (size_t i = 0; i < iBoneSize; i++)
	{
		SaveWString(m_vecBone[i].strName,_pFile);
		fwrite(&m_vecBone[i].iDepth, sizeof(int), 1, _pFile);
		fwrite(&m_vecBone[i].iParent, sizeof(int), 1, _pFile);
		fwrite(&m_vecBone[i].TransForm, sizeof(Mtx), 1, _pFile);
		fwrite(&m_vecBone[i].OffsetMtx, sizeof(Mtx), 1, _pFile);
		UINT iFrmSize = m_vecBone[i].vecKeyFrm.size();
		fwrite(&iFrmSize, sizeof(UINT), 1, _pFile);
		for (size_t j = 0; j < iFrmSize; j++)
		{
			fwrite(&m_vecBone[i].vecKeyFrm[j],sizeof(tMtxKeyFrm),1,_pFile);
		}
	}
}

CMesh* CMesh::LoadMeshData(FILE * _pFile)
{
	wstring strKey = LoadWString(_pFile);
	int iVtxSize = 0;
	int iVtxCount = 0;
	int iByteSize = 0;
	fread(&iVtxSize, sizeof(int), 1, _pFile);
	fread(&iVtxCount, sizeof(int), 1, _pFile);
	fread(&iByteSize, sizeof(int), 1, _pFile);
	
	void* pVtxMem = malloc(iByteSize);
	fread(pVtxMem, iByteSize, 1, _pFile);

	D3D11_BUFFER_DESC descVB;
	fread(&descVB, sizeof(D3D11_BUFFER_DESC), 1, _pFile);

	int iMtrlCount = 0;
	fread(&iMtrlCount, sizeof(UINT), 1, _pFile);

	vector<tIndexInfo> vecInfo;
	for (size_t i = 0; i < iMtrlCount; i++)
	{
		tIndexInfo Info = {};
		fread(&Info, sizeof(tIndexInfo), 1, _pFile);

		void * pSysMem = malloc(Info.iIdxSize * Info.iIdxCount);
		Info.pSysMem = pSysMem;
		fread(Info.pSysMem, Info.iIdxSize * Info.iIdxCount, 1, _pFile);
		if (Info.iIdxCount != 0)
			vecInfo.push_back(Info);
		else
			free(pSysMem);
	}

	UINT iAnimSize = 0;
	fread(&iAnimSize, sizeof(UINT), 1, _pFile);

	vector<tMtxAnimClip> vecAnimClip;
	for (size_t i = 0; i < iAnimSize; i++)
	{
		tMtxAnimClip Clip = {};
		Clip.strName = LoadWString(_pFile);
		fread(&Clip.dStartTime, sizeof(double), 1, _pFile);
		fread(&Clip.dEndTime, sizeof(double), 1, _pFile);
		fread(&Clip.dTimeLength, sizeof(double), 1, _pFile);
		fread(&Clip.iStartFrm, sizeof(int), 1, _pFile);
		fread(&Clip.iEndFrm, sizeof(int), 1, _pFile);
		fread(&Clip.iFrmLength, sizeof(int), 1, _pFile);
		fread(&Clip.eMode, sizeof(int), 1, _pFile);

		vecAnimClip.push_back(Clip);
	}

	UINT iBoneSize = 0;
	fread(&iBoneSize, sizeof(UINT), 1, _pFile);

	vector<tMtxBone> vecBone;
	vecBone.resize(iBoneSize);
	for (size_t i = 0; i < iBoneSize; i++)
	{
		vecBone[i].strName = LoadWString(_pFile);
		fread(&vecBone[i].iDepth, sizeof(int), 1, _pFile);
		fread(&vecBone[i].iParent, sizeof(int), 1, _pFile);
		fread(&vecBone[i].TransForm, sizeof(Mtx), 1, _pFile);
		fread(&vecBone[i].OffsetMtx, sizeof(Mtx), 1, _pFile);

		UINT iFrmSize = 0;
		fread(&iFrmSize, sizeof(UINT), 1, _pFile);
		vecBone[i].vecKeyFrm.resize(iFrmSize);
		for (size_t j = 0; j < iFrmSize; j++)
		{
			fread(&vecBone[i].vecKeyFrm[j], sizeof(tMtxKeyFrm), 1, _pFile);
		}
	}
	
	
	CMesh* pMesh = CResMgr::GetInst()->Load<CMesh>(strKey);
	if (pMesh == nullptr)
	{
		pMesh = CreateMesh(pVtxMem, iVtxSize, iVtxCount, vecInfo);
		pMesh->m_vecAnimClip = vecAnimClip;
		pMesh->m_vecBone = vecBone;
		CResMgr::GetInst()->AddResource<CMesh>(strKey, pMesh);
	}
	else
	{
		free(pVtxMem);

		for (size_t i = 0; i < vecInfo.size(); i++)
		{
			free(vecInfo[i].pSysMem);
		}
	}
	return pMesh;
}

void CMesh::SetBoneTex(CTexture* _pTex) 
{ 
	m_pBoneTex = _pTex; 
}

CMesh::CMesh()
	:m_pVSBlob(nullptr)
	, m_pDrawVB(nullptr)
	, m_pStreamVB(nullptr)
	, m_bStreamVB(false)
	, m_bFirstParticle(true)
	, m_iOffet(0)
	, m_iOffetInst(0)
	, m_pVtxMem(nullptr)
{
}

CMesh::~CMesh()
{
	SAFE_RELEASE(m_pVSBlob);
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pLayout);
	SAFE_RELEASE(m_pDrawVB);
	SAFE_RELEASE(m_pStreamVB);

	for (size_t i = 0; i < m_vecIdxInfo.size(); i++)
	{
		SAFE_RELEASE(m_vecIdxInfo[i].pIB);
		SAFE_DELETE(m_vecIdxInfo[i].pSysMem);
	}

	wstring strName = GetName();

	SAFE_DELETE(m_pVtxMem);
	m_vecLayout.clear();
	SAFE_DELETE(m_pBoneTex);
}