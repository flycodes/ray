// +----------------------------------------------------------------------
// | Project : ray.
// | All rights reserved.
// +----------------------------------------------------------------------
// | Copyright (c) 2013-2015.
// +----------------------------------------------------------------------
// | * Redistribution and use of this software in source and binary forms,
// |   with or without modification, are permitted provided that the following
// |   conditions are met:
// |
// | * Redistributions of source code must retain the above
// |   copyright notice, this list of conditions and the
// |   following disclaimer.
// |
// | * Redistributions in binary form must reproduce the above
// |   copyright notice, this list of conditions and the
// |   following disclaimer in the documentation and/or other
// |   materials provided with the distribution.
// |
// | * Neither the name of the ray team, nor the names of its
// |   contributors may be used to endorse or promote products
// |   derived from this software without specific prior
// |   written permission of the ray team.
// |
// | THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// | "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// | LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// | A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// | OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// | SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// | LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// | DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// | THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// | (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// | OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// +----------------------------------------------------------------------
#ifndef _H_RTTI_MACRO_H_
#define _H_RTTI_MACRO_H_

#include <ray/platform.h>

_NAME_BEGIN

namespace rtti
{

#define __DeclareInterface(Base)\
public:\
	static _NAME rtti::Rtti RTTI;\
	static _NAME rtti::Rtti* getRtti() noexcept;\
    virtual _NAME rtti::Rtti* rtti() const noexcept;\
private:

#define __ImplementInterface(Base, Name) \
    _NAME rtti::Rtti Base::RTTI = _NAME rtti::Rtti(Name, nullptr, nullptr);\
	_NAME rtti::Rtti* Base::rtti() const noexcept { return &RTTI; }\
	static _NAME rtti::Rtti* Base::getRtti() noexcept { return &RTTI; }

#define __DeclareSubInterface(Derived, Base)\
public:\
	static _NAME rtti::Rtti RTTI;\
	static _NAME rtti::Rtti* getRtti() noexcept;\
    virtual _NAME rtti::Rtti* rtti() const noexcept;\
private:

#define __ImplementSubInterface(Derived, Base, Name) \
    _NAME rtti::Rtti Derived::RTTI = _NAME rtti::Rtti(Name, nullptr, Base::getRtti());\
	_NAME rtti::Rtti* Derived::rtti() const noexcept { return &RTTI; }\
	_NAME rtti::Rtti* Derived::getRtti() noexcept { return &RTTI; }

#define __DeclareClass(Base) \
public:\
	static _NAME rtti::Rtti RTTI;\
	static _NAME rtti::Rtti* getRtti() noexcept;\
	static _NAME rtti::Interface* FactoryCreate(); \
    virtual _NAME rtti::Rtti* rtti() const noexcept;\
private:

#define __ImplementClass(Base, Name) \
    _NAME rtti::Rtti Base::RTTI = _NAME rtti::Rtti(Name, Base::FactoryCreate, nullptr);\
	_NAME rtti::Rtti* Base::rtti() const noexcept { return &RTTI; }\
	_NAME rtti::Interface* Base::FactoryCreate() { return new Base; } \
	_NAME rtti::Rtti* Base::getRtti() noexcept { return &RTTI; }

#define __DeclareSubClass(Derived, Base) \
public:\
    static _NAME rtti::Rtti RTTI;\
	static _NAME rtti::Rtti* getRtti() noexcept;\
	static _NAME rtti::Interface* FactoryCreate(); \
    virtual _NAME rtti::Rtti* rtti() const noexcept;\
private:

#define __ImplementSubClass(Derived, Base, Name) \
    _NAME rtti::Rtti Derived::RTTI = _NAME rtti::Rtti(Name, Derived::FactoryCreate, Base::getRtti());\
	_NAME rtti::Rtti* Derived::rtti() const noexcept { return &RTTI; }\
	_NAME rtti::Interface* Derived::FactoryCreate() { return new Derived; }\
	_NAME rtti::Rtti* Derived::getRtti() noexcept { return &RTTI; }
}

_NAME_END

#endif