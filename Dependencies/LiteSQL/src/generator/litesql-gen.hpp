/* XML processor/application API for litesql.dtd.
 * Generated 2005/12/06 13:13:10.
 *
 * This program was generated with the FleXML XML processor generator.
 * FleXML is Copyright � 1999-2005 Kristoffer Rose.  All rights reserved.
 * (Id: flexml.pl,v 1.44 2005/02/23 23:08:16 mquinson Exp).
 * 
 * There are two, intertwined parts to this program, part A and part B.
 *
 * Part A
 * ------
 * 
 * Some parts, here collectively called "Part A", are found in the 
 * FleXML package.  They are Copyright � 1999-2005 Kristoffer Rose. 
 * All rights reserved.
 *
 * You can redistribute, use, perform, display and/or modify "Part A"
 * provided the following two conditions hold:
 *
 * 1. The program is distributed WITHOUT ANY WARRANTY from the author of
 *    FleXML; without even the implied warranty of MERCHANTABILITY or
 *    FITNESS FOR A PARTICULAR PURPOSE.
 *
 * 2. The program distribution conditions do not in any way affect the
 *    distribution conditions of the FleXML system used to generate this
 *    file or any version of FleXML derived from that system.
 *
 * Notice that these are explicit rights granted to you for files
 * generated by the FleXML system.  For your rights in connection with
 * the FleXML system itself please consult the GNU General Public License.
 * 
 * Part B
 * ------
 * 
 * The other parts, here collectively called "Part B", and which came 
 * from the DTD used by FleXML to generate this program, can be 
 * distributed (or not, as the case may be) under the terms of whoever
 * wrote them, provided these terms respect and obey the two conditions 
 * above under the heading "Part A".
 *
 * The author of and contributors to FleXML specifically disclaim
 * any copyright interest in "Part B", unless "Part B" was written 
 * by the author of or contributors to FleXML.
 * 
 */

#ifndef _FLEXML_litesql_H
#define _FLEXML_litesql_H

/* XML application entry points. */
extern void STag_database(void);
extern void ETag_database(void);
extern void STag_object(void);
extern void ETag_object(void);
extern void STag_field(void);
extern void ETag_field(void);
extern void STag_index(void);
extern void ETag_index(void);
extern void STag_indexfield(void);
extern void ETag_indexfield(void);
extern void STag_value(void);
extern void ETag_value(void);
extern void STag_method(void);
extern void ETag_method(void);
extern void STag_param(void);
extern void ETag_param(void);
extern void STag_relation(void);
extern void ETag_relation(void);
extern void STag_relate(void);
extern void ETag_relate(void);

/* XML application data. */
typedef const char* AT_indexfield_name;
#define AU_indexfield_name NULL
typedef const char* AT_relation_id;
#define AU_relation_id NULL
typedef const char* AT_value_name;
#define AU_value_name NULL
typedef enum { AU_relate_limit, A_relate_limit_one,A_relate_limit_many } AT_relate_limit;
typedef const char* AT_object_name;
#define AU_object_name NULL
typedef const char* AT_relate_object;
#define AU_relate_object NULL
typedef const char* AT_database_include;
#define AU_database_include NULL
typedef enum { AU_field_indexed, A_field_indexed_true,A_field_indexed_false } AT_field_indexed;
typedef const char* AT_field_name;
#define AU_field_name NULL
typedef enum { AU_relate_unique, A_relate_unique_true,A_relate_unique_false } AT_relate_unique;
typedef const char* AT_value_value;
#define AU_value_value NULL
typedef const char* AT_database_name;
#define AU_database_name NULL
typedef const char* AT_object_inherits;
#define AU_object_inherits NULL
typedef const char* AT_database_namespace;
#define AU_database_namespace NULL
typedef enum { AU_relation_unidir, A_relation_unidir_true,A_relation_unidir_false } AT_relation_unidir;
typedef const char* AT_param_name;
#define AU_param_name NULL
typedef enum { AU_index_unique, A_index_unique_true,A_index_unique_false } AT_index_unique;
typedef const char* AT_method_name;
#define AU_method_name NULL
typedef enum { AU_field_type, 
               A_field_type_boolean,
               A_field_type_integer,
               A_field_type_string,
               A_field_type_float,
               A_field_type_double,
               A_field_type_time,
               A_field_type_date,
               A_field_type_datetime,
               A_field_type_blob 
             } AT_field_type;
typedef const char* AT_field_default;
#define AU_field_default NULL
typedef const char* AT_method_returntype;
#define AU_method_returntype NULL
typedef const char* AT_relation_name;
#define AU_relation_name NULL
typedef const char* AT_param_type;
#define AU_param_type NULL
typedef const char* AT_relate_handle;
#define AU_relate_handle NULL
typedef enum { AU_field_unique, A_field_unique_true,A_field_unique_false } AT_field_unique;

/* FleXML-provided data. */
extern const char* pcdata;
extern AT_indexfield_name A_indexfield_name;
extern AT_relation_id A_relation_id;
extern AT_value_name A_value_name;
extern AT_relate_limit A_relate_limit;
extern AT_object_name A_object_name;
extern AT_relate_object A_relate_object;
extern AT_database_include A_database_include;
extern AT_field_indexed A_field_indexed;
extern AT_field_name A_field_name;
extern AT_relate_unique A_relate_unique;
extern AT_value_value A_value_value;
extern AT_database_name A_database_name;
extern AT_object_inherits A_object_inherits;
extern AT_database_namespace A_database_namespace;
extern AT_relation_unidir A_relation_unidir;
extern AT_param_name A_param_name;
extern AT_index_unique A_index_unique;
extern AT_method_name A_method_name;
extern AT_field_type A_field_type;
extern AT_field_default A_field_default;
extern AT_method_returntype A_method_returntype;
extern AT_relation_name A_relation_name;
extern AT_param_type A_param_type;
extern AT_relate_handle A_relate_handle;
extern AT_field_unique A_field_unique;

/* XML application utilities. */
extern int element_context(int);

/* XML processor entry point. */
extern int yylex(void);

/* Flexml error handling function (useful only when -q flag passed to flexml) */
const char * parse_err_msg(void);
#endif
