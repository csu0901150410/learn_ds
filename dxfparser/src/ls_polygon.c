#include <stdlib.h> // for malloc
#include <assert.h> // for assert

#include "ls_polygon.h"

lsPolygon *ls_polygon_create()
{
    lsPolygon *polygon = (lsPolygon*)malloc(sizeof(lsPolygon));
    assert(polygon);
    
    polygon->edges = ls_list_create();
    return polygon;
}

void ls_polygon_destroy(lsPolygon **root)
{
    if (NULL == root || NULL == *root)
        return;
        
    // 释放链表上的边
    for (lsListIterator it = ls_list_iterator_start((*root)->edges); !ls_list_iterator_done(&it); ls_list_iterator_step(&it))
    {
        lsLineSegment *pSeg = (lsLineSegment*)ls_list_iterator_get_data(&it);
        ls_line_segment_destroy(&pSeg);
    }

    // 释放链表
    ls_list_destroy(&(*root)->edges);

    // 释放lsPolygon
    free(*root);
    *root = NULL;
}

void ls_polygon_append_seg(lsPolygon *polygon, lsLineSegment seg)
{
    if (NULL == polygon || NULL == polygon->edges)
        return;

    lsLineSegment *pSeg = ls_line_segment_create(seg.s, seg.e);
    ls_list_append(polygon->edges, pSeg);
}

lsBox ls_polygon_get_box(const lsPolygon *polygon)
{
    lsBox box;
    ls_box_init(&box);
    
    if (NULL == polygon || NULL == polygon->edges)
        return box;

    for (lsListIterator it = ls_list_iterator_start(polygon->edges); !ls_list_iterator_done(&it); ls_list_iterator_step(&it))
    {
        // 注意，后续polygon会存入圆弧，这些取polygon数据的逻辑肯定得改的
        const lsLineSegment *pSeg = (lsLineSegment*)ls_list_iterator_get_data(&it);
        if (NULL == pSeg)
            continue;
            
        lsBox segbox = ls_line_segment_get_box(pSeg);
        ls_box_combine(&box, &segbox);
    }

    return box;
}
