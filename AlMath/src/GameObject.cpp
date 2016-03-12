#include <Jasper\GameObject.h>

namespace Jasper {

using namespace std;

GameObject::~GameObject()
{
	if (!m_isDestroyed) {
		Destroy();
	}
}


void GameObject::Initialize() {
	m_transform = Transform();
	m_transform.SetIdentity();
	m_transform.Position = { 0.0f, 0.0f, 0.0f };
}


GameObject& GameObject::AttachChild(std::unique_ptr<GameObject> child) {
	child->SetParemt(this);
	child->SetScene(this->m_scene);
	m_children.push_back(move(child));
	return *this;
}

Component* GameObject::DetachComponent(Component& comp) {
	auto found = find_if(begin(m_components), end(m_components),
		[&comp](const unique_ptr<Component>& c) {
		return c.get() == &comp;
	});
	if (found != end(m_components)) {
		auto result = move(*found);
		result->SetGameObject(nullptr);
		m_components.erase(found);
		return result.get();
	}
	else {
		return nullptr;
	}
}

unique_ptr<GameObject> GameObject::DetachChild(const GameObject& child) {

	auto found = find_if(begin(m_children), end(m_children),
		[&child](const unique_ptr<GameObject>& ch) {
		return ch.get() == &child;
	});

	if (found != end(m_children)) {
		auto result = move(*found);
		result->SetParemt(nullptr);
		m_children.erase(found);
		return result;
	}
	else {
		return nullptr;
	}
}

GameObject& GameObject::AttachNewChild(const std::string& name)
{
	auto child = make_unique<GameObject>(name);
	child->SetParemt(this);
	child->SetScene(this->m_scene);
	GameObject* ret = child.get();
	m_children.push_back(move(child));
	return *ret;
}

Transform GameObject::GetWorldTransform() const{
	Transform result = m_transform;
	GameObject* p = this->m_parent;
	while (p != nullptr) {
		result = p->m_transform * result;
		p = p->m_parent;
	}
	//result.SetScale(m_transform.Scale());
	return result;
}

Component* GameObject::FindComponentByName(std::string name) {
	auto res = find_if(begin(m_components), end(m_components),
		[&](const std::unique_ptr<Component>& c)
	{
		return c->GetName() == name;
	}
	);
	if (res != end(m_components)) {
		return res->get();
	}
	return nullptr;
}

GameObject* GameObject::FindChildByName(std::string name) {
	auto res = find_if(begin(m_children), end(m_children),
		[&](const unique_ptr<GameObject>& c)
	{
		return c->GetName() == name;
	}
	);
	if (res != end(m_children)) {
		return (*res).get();
	}
	return nullptr;
}

Component* GameObject::GetComponentByID(int id)
{
	return nullptr;
}

void GameObject::Awake() {
	AwakeCurrent();
	AwakeChildren();
}

void GameObject::Start() {
	StartCurrent();
	StartChildren();
}

void GameObject::FixedUpdate() {

}

void GameObject::Update(float dt) {
	UpdateCurrent(dt);
	UpdateChildren(dt);
}

void GameObject::Destroy() {
	DestroyCurrent();
	DestroyChildren();
	m_components.clear();
	m_children.clear();
	m_isDestroyed = true;
}


void GameObject::LateUpdate() {

}

void GameObject::AwakeCurrent() {
	for (auto& comp : m_components) {
		comp->Awake();
	}
}

void GameObject::AwakeChildren() {
	for (auto& child : m_children) {
		child->Awake();
	}
}

void GameObject::StartCurrent() {
	// NOOP
}

void GameObject::StartChildren() {
	for (auto& child : m_children) {
		child->Start();
	}
}

void GameObject::UpdateCurrent(float dt) {
	for (auto& comp : m_components) {
		comp->Update();
	}
}

void GameObject::UpdateChildren(float dt) {
	for (auto& child : m_children) {
		child->Update(dt);
	}
}
void GameObject::DestroyCurrent()
{
	for (auto& comp : m_components) {
		comp->Destroy();
	}

}
void GameObject::DestroyChildren()
{
	for (auto& child : m_children) {
		child->Destroy();
	}
}
}