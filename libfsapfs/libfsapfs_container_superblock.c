/*
 * The container superblock functions
 *
 * Copyright (C) 2018, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libfsapfs_container_superblock.h"
#include "libfsapfs_debug.h"
#include "libfsapfs_io_handle.h"
#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"
#include "libfsapfs_libfguid.h"

#include "fsapfs_container_superblock.h"

/* Creates a container superblock
 * Make sure the value container_superblock is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_container_superblock_initialize(
     libfsapfs_container_superblock_t **container_superblock,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_container_superblock_initialize";

	if( container_superblock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid container superblock.",
		 function );

		return( -1 );
	}
	if( *container_superblock != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid container superblock value already set.",
		 function );

		return( -1 );
	}
	*container_superblock = memory_allocate_structure(
	                         libfsapfs_container_superblock_t );

	if( *container_superblock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create container superblock.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *container_superblock,
	     0,
	     sizeof( libfsapfs_container_superblock_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear container superblock.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *container_superblock != NULL )
	{
		memory_free(
		 *container_superblock );

		*container_superblock = NULL;
	}
	return( -1 );
}

/* Frees a container superblock
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_container_superblock_free(
     libfsapfs_container_superblock_t **container_superblock,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_container_superblock_free";

	if( container_superblock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid container superblock.",
		 function );

		return( -1 );
	}
	if( *container_superblock != NULL )
	{
		memory_free(
		 *container_superblock );

		*container_superblock = NULL;
	}
	return( 1 );
}

/* Reads the container superblock
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_container_superblock_read_file_io_handle(
     libfsapfs_container_superblock_t *container_superblock,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	fsapfs_container_superblock_t container_superblock_data;

	static char *function = "libfsapfs_container_superblock_read_file_io_handle";
	ssize_t read_count    = 0;

	if( container_superblock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid container superblock.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading container superblock at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_offset,
		 file_offset );
	}
#endif
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     file_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek container superblock offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              (uint8_t *) &container_superblock_data,
	              sizeof( fsapfs_container_superblock_t ),
	              error );

	if( read_count != (ssize_t) sizeof( fsapfs_container_superblock_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read container superblock data.",
		 function );

		return( -1 );
	}
	if( libfsapfs_container_superblock_read_data(
	     container_superblock,
	     (uint8_t *) &container_superblock_data,
	     sizeof( fsapfs_container_superblock_t ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read container superblock data.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads the container superblock
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_container_superblock_read_data(
     libfsapfs_container_superblock_t *container_superblock,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function               = "libfsapfs_container_superblock_read_data";
	size_t data_offset                  = 0;
	uint64_t incompatible_feature_flags = 0;
	uint64_t volume_object_identifier   = 0;
	uint32_t maximum_number_of_volumes  = 0;
	uint32_t object_subtype             = 0;
	uint32_t object_type                = 0;
	int object_identifier_index         = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit                = 0;
	uint32_t value_32bit                = 0;
#endif

	if( container_superblock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid container superblock.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( ( data_size < sizeof( fsapfs_container_superblock_t ) )
	 || ( data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: container superblock data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( fsapfs_container_superblock_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	byte_stream_copy_to_uint64_little_endian(
	 ( (fsapfs_container_superblock_t *) data )->object_transaction_identifier,
	 container_superblock->object_transaction_identifier );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fsapfs_container_superblock_t *) data )->object_type,
	 object_type );

	if( object_type != 0x80000001UL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid object type: 0x%08" PRIx32 ".",
		 function,
		 object_type );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 ( (fsapfs_container_superblock_t *) data )->object_subtype,
	 object_subtype );

	if( object_subtype != 0x00000000UL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid object subtype: 0x%08" PRIx32 ".",
		 function,
		 object_subtype );

		return( -1 );
	}
	if( memory_compare(
	     ( (fsapfs_container_superblock_t *) data )->signature,
	     fsapfs_container_signature,
	     4 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid signature.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 ( (fsapfs_container_superblock_t *) data )->block_size,
	 container_superblock->block_size );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsapfs_container_superblock_t *) data )->number_of_blocks,
	 container_superblock->number_of_blocks );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsapfs_container_superblock_t *) data )->incompatible_features_flags,
	 incompatible_feature_flags );

	if( memory_copy(
	     container_superblock->container_identifier,
	     ( (fsapfs_container_superblock_t *) data )->container_identifier,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy container identifier.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint64_little_endian(
	 ( (fsapfs_container_superblock_t *) data )->space_manager_object_identifier,
	 container_superblock->space_manager_object_identifier );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsapfs_container_superblock_t *) data )->object_map_block_number,
	 container_superblock->object_map_block_number );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsapfs_container_superblock_t *) data )->reaper_object_identifier,
	 container_superblock->reaper_object_identifier );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fsapfs_container_superblock_t *) data )->maximum_number_of_volumes,
	 maximum_number_of_volumes );

	data_offset = 184;

	for( object_identifier_index = 0;
	     object_identifier_index < 100;
	     object_identifier_index++ )
	{
		byte_stream_copy_to_uint64_little_endian(
		 &( data[ data_offset ] ),
		 volume_object_identifier );

		data_offset += 8;

		container_superblock->volume_object_identifiers[ object_identifier_index ] = volume_object_identifier;

/* TODO what about non-consecutive volume object identifiers ?*/
		if( volume_object_identifier != 0 )
		{
			container_superblock->number_of_volumes += 1;
		}
	}
	byte_stream_copy_to_uint64_little_endian(
	 ( (fsapfs_container_superblock_t *) data )->key_bag_block_number,
	 container_superblock->key_bag_block_number );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsapfs_container_superblock_t *) data )->key_bag_number_of_blocks,
	 container_superblock->key_bag_number_of_blocks );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fsapfs_container_superblock_t *) data )->metadata_area_number_of_blocks,
	 container_superblock->metadata_area_number_of_blocks );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsapfs_container_superblock_t *) data )->metadata_area_block_number,
	 container_superblock->metadata_area_block_number );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_superblock_t *) data )->object_checksum,
		 value_64bit );
		libcnotify_printf(
		 "%s: object checksum\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_superblock_t *) data )->object_identifier,
		 value_64bit );
		libcnotify_printf(
		 "%s: object identifier\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "%s: object transaction identifier\t\t: %" PRIu64 "\n",
		 function,
		 container_superblock->object_transaction_identifier );

		libcnotify_printf(
		 "%s: object type\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 object_type );

		libcnotify_printf(
		 "%s: object subtype\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 object_subtype );

		libcnotify_printf(
		 "%s: signature\t\t\t\t: %c%c%c%c\n",
		 function,
		 ( (fsapfs_container_superblock_t *) data )->signature[ 0 ],
		 ( (fsapfs_container_superblock_t *) data )->signature[ 1 ],
		 ( (fsapfs_container_superblock_t *) data )->signature[ 2 ],
		 ( (fsapfs_container_superblock_t *) data )->signature[ 3 ] );

		libcnotify_printf(
		 "%s: block size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 container_superblock->block_size );

		libcnotify_printf(
		 "%s: number of blocks\t\t\t: %" PRIu64 "\n",
		 function,
		 container_superblock->number_of_blocks );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_superblock_t *) data )->compatible_features_flags,
		 value_64bit );
		libcnotify_printf(
		 "%s: compatible features flags\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );
		libfsapfs_debug_print_container_compatible_feature_flags(
		 value_64bit );
		libcnotify_printf(
		 "\n" );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_superblock_t *) data )->read_only_compatible_features_flags,
		 value_64bit );
		libcnotify_printf(
		 "%s: read-only compatible features flags\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );
		libfsapfs_debug_print_container_read_only_compatible_feature_flags(
		 value_64bit );
		libcnotify_printf(
		 "\n" );

		libcnotify_printf(
		 "%s: incompatible features flags\t\t: 0x%08" PRIx64 "\n",
		 function,
		 incompatible_feature_flags );
		libfsapfs_debug_print_container_incompatible_feature_flags(
		 incompatible_feature_flags );
		libcnotify_printf(
		 "\n" );

		if( libfsapfs_debug_print_guid_value(
		     function,
		     "container identifier\t\t\t",
		     ( (fsapfs_container_superblock_t *) data )->container_identifier,
		     16,
		     LIBFGUID_ENDIAN_BIG,
		     LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print UUID value.",
			 function );

			return( -1 );
		}
		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_superblock_t *) data )->next_object_identifier,
		 value_64bit );
		libcnotify_printf(
		 "%s: next object identifier\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_superblock_t *) data )->next_transaction_identifier,
		 value_64bit );
		libcnotify_printf(
		 "%s: next transaction identifier\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "%s: metadata area number of blocks\t: %" PRIu32 "\n",
		 function,
		 container_superblock->metadata_area_number_of_blocks );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_container_superblock_t *) data )->unknown5,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown5 number of blocks\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: metadata area block number\t\t: %" PRIu64 "\n",
		 function,
		 container_superblock->metadata_area_block_number );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_superblock_t *) data )->unknown7,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown7 block number\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_container_superblock_t *) data )->unknown8,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown8\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_container_superblock_t *) data )->unknown9,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown9\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_container_superblock_t *) data )->unknown10,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown10\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_container_superblock_t *) data )->unknown11,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown11\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_container_superblock_t *) data )->unknown12,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown12\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_container_superblock_t *) data )->unknown13,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown13\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: space manager object identifier\t: %" PRIu64 "\n",
		 function,
		 container_superblock->space_manager_object_identifier );

		libcnotify_printf(
		 "%s: object map block number\t\t: %" PRIu64 "\n",
		 function,
		 container_superblock->object_map_block_number );

		libcnotify_printf(
		 "%s: reaper object identifier\t\t: %" PRIu64 "\n",
		 function,
		 container_superblock->reaper_object_identifier );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_container_superblock_t *) data )->unknown17,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown17\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: maximum number of volumes\t\t: %" PRIu32 "\n",
		 function,
		 maximum_number_of_volumes );

		for( object_identifier_index = 0;
		     object_identifier_index < 100;
		     object_identifier_index++ )
		{
			if( container_superblock->volume_object_identifiers[ object_identifier_index ] != 0 )
			{
				libcnotify_printf(
				 "%s: volume object identifier: %d\t\t: %" PRIu64 "\n",
				 function,
				 object_identifier_index,
				 container_superblock->volume_object_identifiers[ object_identifier_index ] );
			}
		}
		libcnotify_printf(
		 "%s: unknown19:\n",
		 function );
		libcnotify_print_data(
		 ( (fsapfs_container_superblock_t *) data )->unknown19,
		 256,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_superblock_t *) data )->unknown20,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown20\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_superblock_t *) data )->unknown21,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown21\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_superblock_t *) data )->unknown22,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown22\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_superblock_t *) data )->unknown23,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown23\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_superblock_t *) data )->unknown24,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown24\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_superblock_t *) data )->unknown25,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown25\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_superblock_t *) data )->unknown26,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown26\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "%s: key bag block number\t\t\t: %" PRIu64 "\n",
		 function,
		 container_superblock->key_bag_block_number );

		libcnotify_printf(
		 "%s: key bag number of blocks\t\t: %" PRIu64 "\n",
		 function,
		 container_superblock->key_bag_number_of_blocks );

		libcnotify_printf(
		 "%s: unknown29:\n",
		 function );
		libcnotify_print_data(
		 ( (fsapfs_container_superblock_t *) data )->unknown29,
		 40,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_superblock_t *) data )->unknown30,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown30\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_superblock_t *) data )->unknown31,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown31\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_superblock_t *) data )->unknown32,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown32\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_superblock_t *) data )->unknown33,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown33\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( ( incompatible_feature_flags & 0x0000000000000001 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported format version 1.",
		 function );

		return( -1 );
	}
	if( container_superblock->block_size != 4096 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported block size: %" PRIu32 ".",
		 function,
		 container_superblock->block_size );

		return( -1 );
	}
	if( container_superblock->metadata_area_block_number == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported metadata area block number: %" PRIu32 ".",
		 function,
		 container_superblock->metadata_area_block_number );

		return( -1 );
	}
	if( maximum_number_of_volumes > 100 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of volumes value out of bounds.",
		 function );

		return( -1 );
	}
	return( 1 );
}

