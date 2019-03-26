// Fill out your copyright notice in the Description page of Project Settings.

#include "TerrainNode.h"


// Sets default values
ATerrainNode::ATerrainNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Add root component
	RootComponent = CreateAbstractDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Add procedural mesh component 
	TerrainMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("TerrainMesh"));

	// Attach the procedural mesh component to the root
	TerrainMesh->AttachTo(RootComponent);

	// Box node 1
	BoxVisual = CreateDefaultSubobject<UBoxComponent>(TEXT("boxNode1"));
	BoxVisual->AttachTo(RootComponent);


}

// Called in editor
void ATerrainNode::PostActorCreated()
{
	Super::PostActorCreated();


}



//Called when a property is changed
void ATerrainNode::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	//BoxVisual->SetBoxExtent(FVector(TerrainNodeScale, TerrainNodeScale, TerrainNodeScaleZ));

}

// Called when the game starts or when spawned
void ATerrainNode::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATerrainNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckPlayerInBounds();

}

void ATerrainNode::SeekInternalTris()
{
	ia = 0;
	ib = 1;
	ic = 2;
	
	for (int i = 0; i < TerrainVertices.Num(); i++)
	{
		TerrainVertices[i] = TerrainVertices[i] + TerrainLocation;
	}
	// look for and add internal triangles
	for (int i = 0; i < TerrainTriangles.Num() / 3; i++)
	{
		if (TerrainVertices[ia].X  < GetActorLocation().X + TerrainNodeScale &&
			(TerrainVertices[ia].X > GetActorLocation().X - TerrainNodeScale) &&
			(TerrainVertices[ia].Y < GetActorLocation().Y + TerrainNodeScale) &&
			(TerrainVertices[ia].Y > GetActorLocation().Y - TerrainNodeScale) &&

			TerrainVertices[ib].X  < GetActorLocation().X + TerrainNodeScale &&
			(TerrainVertices[ib].X > GetActorLocation().X - TerrainNodeScale) &&
			(TerrainVertices[ib].Y < GetActorLocation().Y + TerrainNodeScale) &&
			(TerrainVertices[ib].Y > GetActorLocation().Y - TerrainNodeScale) &&

			TerrainVertices[ic].X  < GetActorLocation().X + TerrainNodeScale &&
			(TerrainVertices[ic].X > GetActorLocation().X - TerrainNodeScale) &&
			(TerrainVertices[ic].Y < GetActorLocation().Y + TerrainNodeScale) &&
			(TerrainVertices[ic].Y > GetActorLocation().Y - TerrainNodeScale))
		{
			Vertices.Add(TerrainVertices[ia]);
			Vertices.Add(TerrainVertices[ib]);
			Vertices.Add(TerrainVertices[ic]);
		}

		ia = ia + 3;
		ib = ib + 3;
		ic = ic + 3;


	}
	TerrainMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, true);
}



void ATerrainNode::CheckPlayerInBounds()
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

	if (FVector(PlayerPos - TerrainMesh->GetComponentLocation()).Size() < DistFromNode)
	{
		InBounds = true;
	}
	else
	{
		InBounds = false;
	}

}
