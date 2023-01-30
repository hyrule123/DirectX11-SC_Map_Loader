#include "pch.h"
#include "CStructBuffer.h"

#include "CConstBuffer.h"

#include "CDevice.h"


CStructBuffer::CStructBuffer(eSBUFFER_SHARED_CBUFFER_IDX _idx)
	: m_eCBufferIdx(_idx)
	, m_uElementSize()
	, m_uElementCapacity()
	, m_flagPipelineTarget()
	, m_uRegisterIdx()
	, m_BufferDesc{}
{
	//CPU에서 버퍼를 작성하고 GPU에서 버퍼를 읽어들일 경우 사용
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;

	//쉐이더 리소스 속성의 버퍼임
	m_BufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	//CPU에서 구조화버퍼에 작성할 것이므로 AccessFlag를 write로 설정
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	//기타 플래그에 구조화 버퍼로 설정값이 있음.
	m_BufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
}

CStructBuffer::~CStructBuffer()
{
}

void CStructBuffer::Create(UINT _uiElementSize, UINT _uElementCapacity)
{
	//상수버퍼와 마찬가지로 16바이트 단위로 정렬되어 있어야 함.
	assert(0 == _uiElementSize % 16);

	//상수버퍼와는 다르게 버퍼 재할당이 가능함. 먼저 기존 버퍼의 할당을 해제한다.(ComPtr을 통해 관리가 이루어지므로 nullptr로 바꿔주면 됨.)
	m_TempDeleteSB = m_StructBuffer;
	m_StructBuffer = nullptr;
	m_TempDeleteSRV = m_SRV;
	m_SRV = nullptr;

	m_uElementSize = _uiElementSize;
	m_uElementCapacity = _uElementCapacity;

	m_BufferDesc.StructureByteStride = m_uElementSize;
	m_BufferDesc.ByteWidth = m_uElementSize * m_uElementCapacity;

	if (FAILED(DEVICE->CreateBuffer(&m_BufferDesc, nullptr, m_StructBuffer.GetAddressOf())))
		assert(nullptr);


	//SRVDesc에 원소의 갯수를 지정해서 생성.
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	SRVDesc.ViewDimension = D3D_SRV_DIMENSION_BUFFEREX;
	SRVDesc.BufferEx.NumElements = m_uElementCapacity;

	if (FAILED(DEVICE->CreateShaderResourceView(m_StructBuffer.Get(), &SRVDesc, m_SRV.GetAddressOf())))
		assert(nullptr);
}

void CStructBuffer::SetData(void* _pData, UINT _uiCount)
{
	//삭제 대기 상태의 구조화 버퍼가 있을 경우 nullptr을 대입해서 하나씩 삭제(재할당과 삭제 사이에 텀을 줘서 렉 방지)
	if (nullptr != m_TempDeleteSB)
		m_TempDeleteSB = nullptr;
	else if (nullptr != m_TempDeleteSRV)
		m_TempDeleteSRV = nullptr;

	//생성 시 할당된 갯수보다 들어온 갯수가 더 클 경우 재할당하고, 거기에 데이터를 추가.
	if (_uiCount > m_uElementCapacity)
		Create(m_uElementSize, _uiCount * 2u);

	CDevice* pDevice = CDevice::GetInst();

	D3D11_MAPPED_SUBRESOURCE Data = {};
	pDevice->GetDeviceContext()->Map(m_StructBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &Data);
	
	memcpy(Data.pData, _pData, (size_t)(m_uElementSize * _uiCount));

	pDevice->GetDeviceContext()->Unmap(m_StructBuffer.Get(), 0);

	//g_arrStructBufferInfo의 자신의 인덱스에 해당하는 위치에 이번에 업데이트된 구조체의 갯수를 삽입
	g_arrStructBufferInfo[(UINT)m_eCBufferIdx].uSBufferCount = _uiCount;
}

void CStructBuffer::UpdateData()
{
	//구조체 정보를 담은 상수버퍼에 바인딩한 구조체 갯수를 넣어서 전달
	CConstBuffer* pConstBuffer = CDevice::GetInst()->GetConstBuffer(eCONST_BUFFER_SBUFFERINFO);
	pConstBuffer->SetData(g_arrStructBufferInfo);
	pConstBuffer->SetPipelineTarget(m_flagPipelineTarget);
	pConstBuffer->UpdateData();

	if (eSHADER_PIPELINE_FLAG_VERTEX & m_flagPipelineTarget)
	{
		CONTEXT->VSSetShaderResources(m_uRegisterIdx, 1, m_SRV.GetAddressOf());
	}

	if (eSHADER_PIPELINE_FLAG_HULL & m_flagPipelineTarget)
	{
		CONTEXT->HSSetShaderResources(m_uRegisterIdx, 1, m_SRV.GetAddressOf());
	}

	if (eSHADER_PIPELINE_FLAG_DOMAIN & m_flagPipelineTarget)
	{
		CONTEXT->DSSetShaderResources(m_uRegisterIdx, 1, m_SRV.GetAddressOf());
	}

	if (eSHADER_PIPELINE_FLAG_GEOMETRY & m_flagPipelineTarget)
	{
		CONTEXT->GSSetShaderResources(m_uRegisterIdx, 1, m_SRV.GetAddressOf());
	}

	if (eSHADER_PIPELINE_FLAG_PIXEL & m_flagPipelineTarget)
	{
		CONTEXT->PSSetShaderResources(m_uRegisterIdx, 1, m_SRV.GetAddressOf());
	}
}
