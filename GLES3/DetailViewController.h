//
//  DetailViewController.h
//  GLES3
//
//  Created by George on 17/1/4.
//  Copyright © 2017年 George. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface DetailViewController : UIViewController

@property (strong, nonatomic) id detailItem;
@property (weak, nonatomic) IBOutlet UILabel *detailDescriptionLabel;

@end

