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
#ifndef _H_GAME_OBJECT_H_
#define _H_GAME_OBJECT_H_

#include <ray/game_types.h>

_NAME_BEGIN

class EXPORT GameObject : public rtti::Interface
{
	__DeclareSubClass(GameObject, rtti::Interface)
public:
	GameObject() noexcept;
	virtual ~GameObject() noexcept;

	void setName(const std::string& name) noexcept;
	const std::string& getName() const noexcept;

	void setActive(bool active) except;
	void setActiveUpwards(bool active) except;
	void setActiveDownwards(bool active) except;
	bool getActive() const noexcept;

	void setLayer(std::uint8_t layer) noexcept;
	std::uint8_t getLayer() const noexcept;

	std::uint32_t getInstanceID() const noexcept;

	void setParent(GameObjectPtr parent) noexcept;
	GameObjectPtr getParent() const noexcept;

	void addChild(GameObjectPtr child) noexcept;
	void removeChild(GameObjectPtr child) noexcept;
	void cleanupChildren() noexcept;
	GameObjectPtr findChild(const std::string& name, bool recurse = true) noexcept;

	std::uint32_t getChildCount() const noexcept;
	GameObjects& getChildren() noexcept;
	const GameObjects& getChildren() const noexcept;

	void setTranslateX(float dist) noexcept;
	void setTranslateY(float dist) noexcept;
	void setTranslateZ(float dist) noexcept;
	void setTranslateOnAxis(float dist, const Vector3& axis) noexcept;
	void setTranslateAccum(float v) noexcept;
	void setTranslateAccum(float x, float y, float z) noexcept;
	void setTranslateAccumX(float dist) noexcept;
	void setTranslateAccumY(float dist) noexcept;
	void setTranslateAccumZ(float dist) noexcept;
	void setTranslateAccumOnAxis(float dist, const Vector3& axis) noexcept;
	void setTranslateAccum(const Vector3& v) noexcept;
	void setTranslate(const Vector3& v) noexcept;

	const Vector3& getTranslate() const noexcept;

	void setScaleX(float dist) noexcept;
	void setScaleY(float dist) noexcept;
	void setScaleZ(float dist) noexcept;
	void setScaleOnAxis(float dist, const Vector3& axis) noexcept;
	void setScaleAccum(float v) noexcept;
	void setScaleAccum(float x, float y, float z) noexcept;
	void setScaleAccumX(float dist) noexcept;
	void setScaleAccumY(float dist) noexcept;
	void setScaleAccumZ(float dist) noexcept;
	void setScaleAccumOnAxis(float dist, const Vector3& axis) noexcept;
	void setScaleAccum(const Vector3& v) noexcept;
	void setScale(const Vector3& v) noexcept;

	const Vector3& getScale() const noexcept;

	void setRotateX(float dist) noexcept;
	void setRotateY(float dist) noexcept;
	void setRotateZ(float dist) noexcept;
	void setRotateOnAxis(float dist, const Vector3& axis) noexcept;
	void setRotateAccum(float v) noexcept;
	void setRotateAccum(float x, float y, float z) noexcept;
	void setRotateAccumX(float dist) noexcept;
	void setRotateAccumY(float dist) noexcept;
	void setRotateAccumZ(float dist) noexcept;
	void setRotateAccumOnAxis(float dist, const Vector3& axis) noexcept;
	void setRotateAccum(const Quaternion& v) noexcept;
	void setRotate(const Quaternion& v) noexcept;
	void setRotate(float x, float y, float z) noexcept;
	const Quaternion& getRotate() const noexcept;

	void setRight(const Vector3& v) noexcept;
	const Vector3& getRight() const noexcept;

	void setUpVector(const Vector3& v) noexcept;
	const Vector3& getUpVector() const noexcept;

	void setForward(const Vector3& v) noexcept;
	const Vector3& getForward() const noexcept;

	const Matrix4x4& getTransform() const noexcept;
	const Matrix4x4& getTransformInverse() const noexcept;
	const Matrix4x4& getTransformInverseTranspose() const noexcept;

	void addComponent(GameComponentPtr component) except;
	void removeComponent(GameComponentPtr component) noexcept;
	void cleanupComponents() noexcept;

	template<typename T>
	std::shared_ptr<T> getComponent() const noexcept { return std::dynamic_pointer_cast<T>(this->getComponent(T::RTTI)); }
	GameComponentPtr getComponent(const rtti::Rtti* type) const noexcept;
	GameComponentPtr getComponent(const rtti::Rtti& type) const noexcept;

	template<typename T>
	std::shared_ptr<T> getComponentInChildren() const noexcept { return std::dynamic_pointer_cast<T>(this->getComponentInChildren(T::RTTI)); }
	GameComponentPtr getComponentInChildren(const rtti::Rtti* type) const noexcept;
	GameComponentPtr getComponentInChildren(const rtti::Rtti& type) const noexcept;

	const GameComponents& getComponents() const noexcept;

	void sendMessage(const MessagePtr& message) noexcept;
	void sendMessageUpwards(const MessagePtr& message) noexcept;
	void sendMessageDownwards(const MessagePtr& message) noexcept;

	void destroy() noexcept;

	void load(iarchive& reader) noexcept;
	void save(oarchive& write) noexcept;

	GameObjectPtr clone() const except;

	virtual GameServer* getGameServer() noexcept;
	virtual GameScene* getGameScene() noexcept;

private:

	friend class GameObjectManager;

	void _onFrameBegin() except;
	void _onFrame() except;
	void _onFrameEnd() except;

	void _onMoveBefore() except;
	void _onMoveAfter() except;

	void _updateTransform() const noexcept;

private:
	GameObject(const GameObject& copy) noexcept = delete;
	GameObject& operator=(const GameObject& copy) noexcept = delete;

private:

	bool _active;

	std::uint8_t _layer;
	std::uint32_t _instanceID;

	std::string _name;

	Vector3 _translate;
	Vector3 _scaling;
	Quaternion _quat;

	Vector3 _right;
	Vector3 _up;
	Vector3 _forward;

	mutable bool _needUpdates;

	mutable Matrix4x4 _transform;
	mutable Matrix4x4 _transformInverse;
	mutable Matrix4x4 _transformInverseTranspose;

	GameObjects _children;
	GameObjectWeakPtr _parent;

	GameComponents _components;
};

_NAME_END

#endif