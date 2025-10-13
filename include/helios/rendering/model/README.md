# helios.rendering.model

Contains (abstract) classes in regard to rendering [shapes](../asset/shape/README.md) consisting of geometric [primitives](../core/README.md).

## Hierarchical model

### Renderable
A Renderable owns the **Mesh** and also has **Material** information. 
**Material** is shared with other entities. A Renderable has no hierarchical information.


#### Mesh 
 - `stateful`
 - `unique` \
A **Mesh** shares **MeshData** with other entities. The purpose of **Mesh** is to provide a unique, stateful instance, exclusively owned by a **Renderable**.

##### MeshData
- `stateless / const`
- `shared` \
**MeshData** is shared by entities and provides [**Vertex**](../core/Vertex.ixx)-Information and _indices:-

#### Material
- `stateful`
- `unique` \
A **Material** shares **MaterialData** with other entities. The purpose of **Material** is to provide a unique,  stateful instance, exclusively owned by a **Renderable**.

#### MaterialData
- `stateless / const`
- `shared` \
**MaterialData** encapsulates a **Shader** and is shared among multiple entities. To this time, no further material properties are provided with the implementation


##### Shader
- `stateful`
- `shared`\
Abstraction to provide loading and compilation of vertex- and fragment-shader, as well as state-relevant commands for the rendering pipeline (e.g. `use`). 