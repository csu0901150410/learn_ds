#include "../src/ls_list.h"
#include "../src/ls_entity.h"

#include <stdio.h>
#include <stdlib.h>

//int main()
//{
//    lsList *list = ls_list_create();
//
//    lsPoint start, end, center;
//
//    start = ls_point_create(0, 0);
//    end = ls_point_create(1, 1);
//    ls_list_append(list, ls_entity_create_segment(start, end));
//
//    start = ls_point_create(2, 2);
//    end = ls_point_create(3, 3);
//    ls_list_append(list, ls_entity_create_segment(start, end));
//
//    center = ls_point_create(5, 6);
//    ls_list_append(list, ls_entity_create_arc(start, end, center, true));
//
//    // 遍历链表，取出图元，打印信息
//    for (lsListIterator it = ls_list_iterator_start(list); !ls_list_iterator_done(&it); ls_list_iterator_step(&it))
//    {
//        lsEntity *entity = ls_list_iterator_get_data(&it);
//        if (NULL == entity)
//            continue;
//
//        if (enum_geo_segment == entity->type)
//        {
//            lsLineSegment *seg = entity->entity;
//            printf("segment: start(%.3f, %.3f), end(%.3f, %.3f)\n", seg->s.x, seg->s.y, seg->e.x, seg->e.y);
//        }
//        else if (enum_geo_arc == entity->type)
//        {
//            lsArc *arc = entity->entity;
//            printf("arc: start(%.3f, %.3f), end(%.3f, %.3f), center(%.3f, %.3f), bccw(%s)\n", arc->s.x, arc->s.y, 
//                    arc->e.x, arc->e.y, arc->c.x, arc->c.y, arc->bccw ? "true" : "false");
//        }
//    }
//
//    // 销毁实体
//    for (lsListIterator it = ls_list_iterator_start(list); !ls_list_iterator_done(&it); ls_list_iterator_step(&it))
//    {
//        lsEntity *entity = ls_list_iterator_get_data(&it);
//        if (NULL == entity)
//            continue;
//        ls_entity_destroy(&entity);
//    }
//
//    // 销毁链表
//    ls_list_destroy(&list);
//
//    return 0;
//}
