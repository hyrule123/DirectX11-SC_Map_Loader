#pragma once
#include "CTilemap.h"
class CTilemapAtlas :
    public CTilemap
{
public:
    CTilemapAtlas();
    virtual ~CTilemapAtlas();

    CTilemapAtlas(CTilemapAtlas const& _other);
    CLONE(CTilemapAtlas);

public:
    //virtual void init();
    virtual void finaltick() override;
    virtual bool render() override;
    virtual void cleanup() override {};

    virtual void SetTileCount(UINT _iXCount, UINT _iYCount) override;


private:
    Vec2                m_vSliceSize;   // 타일 하나의 크기(UV 단위)
    vector<tTile>       m_vecTile;
    std::unique_ptr<CStructBuffer> m_SBuffer;

public:
    void SetSliceSize(Vec2 _vSliceSize) { m_vSliceSize = _vSliceSize; }

    void BindData();
};

