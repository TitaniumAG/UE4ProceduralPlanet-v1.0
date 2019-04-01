// Fill out your copyright notice in the Description page of Project Settings.

#include "PlanetNode.h"

// Sets default values
APlanetNode::APlanetNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Add root component
	RootComponent = CreateAbstractDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Add procedural mesh component 
	PlanetMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("TerrainMesh"));

	// Attach the procedural mesh component to the root
	PlanetMesh->AttachTo(RootComponent);

	// Box node 1
	BoxFrame = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxNode1"));
	BoxFrame->AttachTo(RootComponent);

	// Set the terrain scale value
	PlanetScale = 500;

	// Set box extent
	NodeScale = 500;

	// Set the search index values
	IndexA = 0;
	IndexB = 1;
	IndexC = 2;

	Ia = 0;
	Ib = 1;
	Ic = 2;


}


void APlanetNode::PostActorCreated()
{
	Super::PostActorCreated();

	//BoxFrame->SetBoxExtent(FVector(NodeScale, NodeScale, NodeScale));

	//PlanetMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, true);

	
}


//Called when a property is changed
void APlanetNode::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	SeekInternalTris();

	//HandleSubdivision();

	//PlanetMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, true);

	BoxFrame->SetBoxExtent(FVector(NodeScale, NodeScale, NodeScale));


}

// Called when the game starts or when spawned
void APlanetNode::BeginPlay()
{
	Super::BeginPlay();

	BoxFrame->SetBoxExtent(FVector(NodeScale, NodeScale, NodeScale));
	
}

// Called every frame
void APlanetNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckPlayerInBounds();

}



void APlanetNode::GenerateMesh()
{

}

void APlanetNode::Subdivide(int32 a, int32 b, int32 c)
{
	FVector va, vb, vc, vab, vbc, vca;


	va = Vertices[a];
	vb = Vertices[b];
	vc = Vertices[c];

	vab = FMath::Lerp(va, vb, 0.5);
	vbc = FMath::Lerp(vb, vc, 0.5);
	vca = FMath::Lerp(vc, va, 0.5);


	Vertices_New.Add(va);
	Vertices_New.Add(vab);
	Vertices_New.Add(vca);

	Vertices_New.Add(vca);
	Vertices_New.Add(vbc);
	Vertices_New.Add(vc);

	Vertices_New.Add(vab);
	Vertices_New.Add(vb);
	Vertices_New.Add(vbc);

	Vertices_New.Add(vab);
	Vertices_New.Add(vbc);
	Vertices_New.Add(vca);


	IndexA = IndexA + 3;
	IndexB = IndexB + 3;
	IndexC = IndexC + 3;

}

void APlanetNode::BuildTriangleList()
{

	for (int i = 0; i < Vertices.Num(); i++)
	{
		// Build vertecx index list
		Triangles.Add(i);
	}
}

void APlanetNode::HandleSubdivision()
{
	//Reset index counters 
	IndexA = 0;
	IndexB = 1;
	IndexC = 2;

	// Handle the subdivisions
	for (int i = 0; i < Recursions; i++)
	{

		for (int j = 0; j < Triangles.Num() / 3; j++)
		{
			Subdivide(Triangles[IndexA], Triangles[IndexB], Triangles[IndexC]);
		}

		// Empty
		Vertices.Empty();
		Triangles.Empty();



		//Assign new to current
		Vertices = Vertices_New;


		//New empty 
		Vertices_New.Empty();
		VertexColors.Empty();

		//Build tri indices
		BuildTriangleList();


	}


}


void APlanetNode::SeekInternalTris()
{
	Ia = 0;
	Ib = 1;
	Ic = 2;

	// The planet Vertices must take the planet location into account
	for (int i = 0; i < PlanetVertices.Num(); i++)
	{
		PlanetVertices[i] = PlanetVertices[i]  + PlanetLocation;
	}

	for (int i = 0; i < PlanetTriangles.Num() / 3; i++)
	{
		// Get average point of current triangle
		FVector avg, ab, bc, ca;
		avg = (PlanetVertices[PlanetTriangles[Ia]] + PlanetVertices[PlanetTriangles[Ib]] + PlanetVertices[PlanetTriangles[Ic]]) / 3;

		ab = FMath::Lerp(PlanetVertices[PlanetTriangles[Ia]], PlanetVertices[PlanetTriangles[Ib]], 0.5);
		bc = FMath::Lerp(PlanetVertices[PlanetTriangles[Ib]], PlanetVertices[PlanetTriangles[Ic]], 0.5);
		ca = FMath::Lerp(PlanetVertices[PlanetTriangles[Ic]], PlanetVertices[PlanetTriangles[Ia]], 0.5);
		
		// Check whether the average point is within the bounds of this node
		
		if(avg.X > BoxFrame->GetComponentLocation().X - NodeScale &&
			avg.X < BoxFrame->GetComponentLocation().X + NodeScale &&
			
			avg.Y > BoxFrame->GetComponentLocation().Y - NodeScale &&
			avg.Y < BoxFrame->GetComponentLocation().Y + NodeScale && 
			
			avg.Z > BoxFrame->GetComponentLocation().Z - NodeScale &&
			avg.Z < BoxFrame->GetComponentLocation().Z + NodeScale ||


			// Edges a
			ab.X > BoxFrame->GetComponentLocation().X - NodeScale &&
			ab.X < BoxFrame->GetComponentLocation().X + NodeScale &&

			ab.Y > BoxFrame->GetComponentLocation().Y - NodeScale &&
			ab.Y < BoxFrame->GetComponentLocation().Y + NodeScale &&

			ab.Z > BoxFrame->GetComponentLocation().Z - NodeScale &&
			ab.Z < BoxFrame->GetComponentLocation().Z + NodeScale ||


			// Edges b
			bc.X > BoxFrame->GetComponentLocation().X - NodeScale &&
			bc.X < BoxFrame->GetComponentLocation().X + NodeScale &&

			bc.Y > BoxFrame->GetComponentLocation().Y - NodeScale &&
			bc.Y < BoxFrame->GetComponentLocation().Y + NodeScale &&

			bc.Z > BoxFrame->GetComponentLocation().Z - NodeScale &&
			bc.Z < BoxFrame->GetComponentLocation().Z + NodeScale ||

			// Edges c
			ca.X > BoxFrame->GetComponentLocation().X - NodeScale &&
			ca.X < BoxFrame->GetComponentLocation().X + NodeScale &&

			ca.Y > BoxFrame->GetComponentLocation().Y - NodeScale &&
			ca.Y < BoxFrame->GetComponentLocation().Y + NodeScale &&

			ca.Z > BoxFrame->GetComponentLocation().Z - NodeScale &&
			ca.Z < BoxFrame->GetComponentLocation().Z + NodeScale)

			{
			Vertices.Add(PlanetVertices[PlanetTriangles[Ia]]);
			Vertices.Add(PlanetVertices[PlanetTriangles[Ib]]);
			Vertices.Add(PlanetVertices[PlanetTriangles[Ic]]);
			}
		
		// Step to the next face/triangle
		Ia = Ia + 3;
		Ib = Ib + 3;
		Ic = Ic + 3;
		
	}
	
	// Clear planet triangles
	PlanetTriangles.Empty();

	// Build Triangle list when done... 
	for (int i = 0; i < Vertices.Num(); i++)
	{
		PlanetTriangles.Add(i);
		Triangles.Add(i);
	}

	// Lets generate the geometry for this region...
	PlanetMesh->SetWorldLocation(PlanetLocation);
	//HandleSubdivision();
	PlanetMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, true);


}



void APlanetNode::CheckPlayerInBounds()
{
	/*
	if (PlayerPos.X < TerrainMesh->GetComponentLocation().X + TerrainScale&&
	PlayerPos.X > TerrainMesh->GetComponentLocation().X - TerrainScale &&
	PlayerPos.Y < TerrainMesh->GetComponentLocation().Y + TerrainScale &&
	PlayerPos.Y > TerrainMesh->GetComponentLocation().Y - TerrainScale)

	{
	InBounds = true;
	}
	else
	{
	InBounds = false;
	}
	*/

	if (FVector(PlayerPos - BoxFrame->GetComponentLocation()).Size() < DistFromPlanetNode)
	{
		InBounds = true;
	}
	else
	{
		InBounds = false;
	}

}

