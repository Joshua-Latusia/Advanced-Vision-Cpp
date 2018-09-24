#pragma once
class TestClass
{
public:
	TestClass();
	~TestClass();

	/// <summary>
	/// Tests the boundary points functions and displays the results for the monsters and co picture.
	/// </summary>
	static void testBoundaryPoints();
	
	/// <summary>
	/// Tests the chain code functions for the monsters and co picture.
	/// </summary>
	static void testChainCode();
	
	/// <summary>
	/// Tests the bounding boxes.
	/// </summary>
	static void testBoundingBoxes();
	
	/// <summary>
	/// Tests the boundary fill.
	/// </summary>
	static void testBoundaryFill();
};

