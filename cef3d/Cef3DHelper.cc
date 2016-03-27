//----------------------------------------------------------------------------
//Yume Engine
//Copyright (C) 2015  arkenthera
//This program is free software; you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation; either version 2 of the License, or
//(at your option) any later version.
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//You should have received a copy of the GNU General Public License along
//with this program; if not, write to the Free Software Foundation, Inc.,
//51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.*/
//----------------------------------------------------------------------------
//
// File : <Filename>
// Date : <Date>
// Comments :
//
//----------------------------------------------------------------------------
#include "Cef3DHelper.h"

#include <sstream>

#include "include/cef_parser.h"
#include "include/base/cef_logging.h"

namespace Cef3DHelpers
{
	// Transfer a V8 value to a List index.
	void
		SetListValue(
		CefRefPtr<CefListValue> list,
		int index,
		CefRefPtr<CefV8Value> value) {

		if(value->IsArray()) {
			CefRefPtr<CefListValue> new_list = CefListValue::Create();
			SetList(value,new_list);
			list->SetList(index,new_list);
		}
		else if(value->IsString()) {
			list->SetString(index,value->GetStringValue());
		}
		else if(value->IsBool()) {
			list->SetBool(index,value->GetBoolValue());
		}
		else if(value->IsInt()) {
			list->SetInt(index,value->GetIntValue());
		}
		else if(value->IsDouble()) {
			list->SetDouble(index,value->GetDoubleValue());
		}
		else if(value->IsNull()) {
			list->SetNull(index);
		}
		else {
			LOG(WARNING) << "Unknown CEF list value type";
		}
	}

	// Transfer a V8 array to a List.
	void
		SetList(CefRefPtr<CefV8Value> source,CefRefPtr<CefListValue> target) {

		int arg_length = source->GetArrayLength();
		if(arg_length == 0)
			return;

		// Start with null types in all spaces.
		target->SetSize(arg_length);

		for(int i = 0; i < arg_length; ++i)
			SetListValue(target,i,source->GetValue(i));
	}

	CefRefPtr<CefV8Value>
		ListValueToV8Value(CefRefPtr<CefListValue> value,int index) {
		CefRefPtr<CefV8Value> new_value;

		CefValueType type = value->GetType(index);

		switch(type) {
		case VTYPE_LIST: {
			CefRefPtr<CefListValue> list = value->GetList(index);
			new_value = CefV8Value::CreateArray(list->GetSize());
			SetList(list,new_value);
		} break;
		case VTYPE_BOOL:
			new_value = CefV8Value::CreateBool(value->GetBool(index));
			break;
		case VTYPE_DOUBLE:
			new_value = CefV8Value::CreateDouble(value->GetDouble(index));
			break;
		case VTYPE_INT:
			new_value = CefV8Value::CreateInt(value->GetInt(index));
			break;
		case VTYPE_STRING:
			new_value = CefV8Value::CreateString(value->GetString(index));
			break;
		default:
			new_value = CefV8Value::CreateNull();
			break;
		}

		return new_value;
	}

	// Transfer a List value to a V8 array index.
	void
		SetListValue(
		CefRefPtr<CefV8Value> list,
		int index,
		CefRefPtr<CefListValue> value) {

		CefRefPtr<CefV8Value> new_value;

		CefValueType type = value->GetType(index);
		switch(type) {
		case VTYPE_LIST: {
			CefRefPtr<CefListValue> new_list = value->GetList(index);
			new_value = CefV8Value::CreateArray(new_list->GetSize());
			SetList(new_list,new_value);
		} break;
		case VTYPE_BOOL:
			new_value = CefV8Value::CreateBool(value->GetBool(index));
			break;
		case VTYPE_DOUBLE:
			new_value = CefV8Value::CreateDouble(value->GetDouble(index));
			break;
		case VTYPE_INT:
			new_value = CefV8Value::CreateInt(value->GetInt(index));
			break;
		case VTYPE_STRING:
			new_value = CefV8Value::CreateString(value->GetString(index));
			break;
		case VTYPE_NULL:
			new_value = CefV8Value::CreateNull();
			break;
		default:
			break;
		}

		if(new_value.get()) {
			list->SetValue(index,new_value);
		}
		else {
			list->SetValue(index,CefV8Value::CreateNull());
		}
	}

	// Transfer a List to a V8 array.
	void
		SetList(CefRefPtr<CefListValue> source,CefRefPtr<CefV8Value> target) {

		int arg_length = source->GetSize();
		if(arg_length == 0)
			return;

		for(int i = 0; i < arg_length; ++i)
			SetListValue(target,i,source);
	}

	std::string
		DumpListValue(CefRefPtr<CefListValue> list,int index) {
		std::string result;
		switch(list->GetType(index)) {
		case VTYPE_LIST:
			result += DumpList(list->GetList(index));
			break;
		case VTYPE_BOOL:
			result += (list->GetBool(index) ? "true" : "false");
			break;
		case VTYPE_DOUBLE:
			result += std::to_string(list->GetDouble(index));
			break;
		case VTYPE_INT:
			result += std::to_string(list->GetInt(index));
			break;
		case VTYPE_STRING:
			result += '"';
			result += list->GetString(index);
			result += '"';
			break;
		case VTYPE_NULL:
			result += "null";
			break;
		default:
			result += "unknown";
		}

		return result;
	}

	std::string
		DumpList(CefRefPtr<CefListValue> list) {
		int arg_length = list->GetSize();
		if(arg_length == 0)
			return "";

		std::stringstream result;
		result << "(";
		for(int i = 0; i < arg_length; ++i) {
			result << DumpListValue(list,i);
			if(i + 1 < arg_length)
				result << ", ";
		}
		result << ")";
		return result.str();
	}

	std::string
		stringReplace(std::string src,std::string const &target,std::string const &repl) {
		// handle error situations/trivial cases

		if(!target.length()) {
			// searching for a match to the empty string will result in
			//  an infinite loop
			//  it might make sense to throw an exception for this case
			return src;
		}

		if(!src.length()) {
			return src;  // nothing to match against
		}

		std::string::size_type idx = 0;

		for(;;) {
			idx = src.find(target,idx);
			if(idx == std::string::npos)
				break;

			src.replace(idx,target.length(),repl);
			idx += repl.length();
		}

		return src;
	}

	// Retrieve the file name and mime type based on the specified url.
	bool
		ParseUrl(const std::string& url,std::string* file_name,std::string* mime_type) {
		// Retrieve the path component.
		CefURLParts parts;
		CefParseURL(url,parts);
		std::string file = CefString(&parts.path);
		if(file.size() < 2)
			return false;

		// Verify that the file name is valid.
		if(file.find("..") != std::string::npos)
			return false;

		for(const auto &c : file) {
			if(!isalpha(c) && !isdigit(c) && c != '_' && c != '-' && c != '.' && c != '/') {
				return false;
			}
		}

		// Determine the mime type based on the file extension, if any.
		std::string ext = GetFileExtension(file);
		if(!ext.empty()) {
			*mime_type = GetMimeType(ext);
		}
		else {
			// Default to an html extension if none is specified.
			*mime_type = "text/html";
			file += ".html";
		}

		*file_name = file;
		return true;
	}

	uint32
		HashString(const std::string& str) {
		// Currently based on FNV-1 with 32 bit prime
		const uint32 fnv_prime = 0x811C9DC5;
		uint32 hash = 0;
		for(std::size_t i = 0; i < str.length(); i++) {
			hash *= fnv_prime;
			hash ^= str[i];
		}

		return hash;
	}

	std::string
		BaseDir(const std::string& source) {
		if(source.size() <= 1) {
			// Make sure it's possible to check the last character.
			return source;
		}

		std::string result;
		if(*(source.rbegin() + 1) == '/') {
			// Remove trailing slash if it exists.
			result = source.substr(0,source.length() - 1);
		}
		else {
			result = source;
		}

		size_t pos = result.find_last_of('/');
		if(pos == std::string::npos)
			return source;

		return result.substr(0,pos);
	}

	std::string
		GetFileExtension(const std::string& source) {
		size_t pos = source.rfind(".");
		if(pos != std::string::npos) {
			return source.substr(pos + 1);
		}

		return "";
	}

	std::string
		GetFileName(const std::string& source) {
		size_t pos = source.rfind("/");
		if(pos != std::string::npos) {
			return source.substr(pos + 1);
		}

		return "";
	}

	std::string
		GetMimeType(const std::string& ext) {
		if(ext == "html")
			return "text/html";
		else if(ext == "png")
			return "image/png";
		else if(ext == "jpg")
			return "image/jpeg";
		else if(ext == "gif")
			return "image/gif";
		else if(ext == "js")
			return "text/javascript";
		else if(ext == "css")
			return "text/css";
		else
			return "application/octet-strem";
	}
}